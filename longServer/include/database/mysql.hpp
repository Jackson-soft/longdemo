#pragma once

#include "database/dsn.hpp"

#include <any>
#include <boost/algorithm/string/replace.hpp>
#include <cstdint>
#include <cstring>
#include <fmt/format.h>
#include <memory>
#include <mysql.h>
#include <string>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <typeinfo>
#include <utility>

namespace uranus::database {
class MySQL
{
public:
    MySQL() = default;

    ~MySQL() {
        Close();
    }

    auto Connect(std::string_view    host,
                 std::string_view    user,
                 std::string_view    pwd,
                 std::string_view    db,
                 const std::uint32_t port = 3306) -> bool {
        if (host.empty() || user.empty()) {
            return false;
        }
        mysql_ = ::mysql_init(nullptr);
        if (mysql_ == nullptr) {
            return false;
        }

        char reconnect = 1;
        // 启用重新连接，必须在调用mysql_real_connect前设置
        if (0 != ::mysql_options(mysql_, MYSQL_OPT_RECONNECT, (void *)&reconnect)) {
            return false;
        }

        if (0 != ::mysql_options(mysql_, MYSQL_SET_CHARSET_NAME, (void *)"utf8mb4")) {
            return false;
        }

        mysql_ = ::mysql_real_connect(mysql_, host.data(), user.data(), pwd.data(), db.data(), port, nullptr, 0);
        return mysql_ != nullptr;
    }

    auto Connect(std::string_view dsn) -> bool {
        if (dsn.empty()) {
            return false;
        }
        return false;
    }

    // error information
    auto Error() -> std::tuple<std::uint32_t, std::string> {
        return {::mysql_errno(mysql_), ::mysql_error(mysql_)};
    }

    // ping
    auto Ping() -> bool {
        return 0 == ::mysql_ping(mysql_);
    }

    auto Timeout(std::uint32_t timeout) -> bool {
        if (timeout <= 0) {
            return false;
        }
        return ::mysql_options(mysql_, MYSQL_OPT_CONNECT_TIMEOUT, (void *)&timeout) == 0;
    }

    auto Insert(const std::string_view sql, const std::vector<std::any> &args) -> std::uint64_t {
        if (sql.empty()) {
            return 0;
        }

        ::MYSQL_STMT *stmt = ::mysql_stmt_init(mysql_);
        if (0 != ::mysql_stmt_prepare(stmt, sql.data(), sql.size())) {
            auto [code, message] = Error();
            return 0;
        }

        auto paramCount = ::mysql_stmt_param_count(stmt);

        if (args.size() != paramCount) {
            return 0;
        }

        std::vector<::MYSQL_BIND> vBinds;
        vBinds.reserve(paramCount);
        // std::memset(vBinds.data(), 0, sizeof(::MYSQL_BIND) * paramCount);
        for (const auto &it : args) {
            ::MYSQL_BIND bind;
            if (typeid(char) == it.type()) {
                bind.buffer_type = MYSQL_TYPE_TINY;
                bind.buffer      = reinterpret_cast<void *>(std::any_cast<char>(it));
            } else if (typeid(const char *) == it.type()) {
                bind.buffer_type   = MYSQL_TYPE_STRING;
                const auto *tmp    = std::any_cast<const char *>(it);
                bind.buffer        = (void *)tmp;
                bind.buffer_length = std::strlen(tmp);
            } else if (typeid(std::string) == it.type()) {
                bind.buffer_type   = MYSQL_TYPE_STRING;
                auto tmp           = std::any_cast<std::string>(it);
                bind.buffer        = tmp.data();
                bind.buffer_length = tmp.size();
            } else if (typeid(int) == it.type()) {
                bind.buffer_type = MYSQL_TYPE_LONG;
                bind.buffer      = reinterpret_cast<void *>(std::any_cast<int>(it));
            }
            vBinds.emplace_back(bind);
        }

        if (::mysql_stmt_bind_param(stmt, vBinds.data())) {
            auto [code, message] = Error();
            return 0;
        }

        if (0 != ::mysql_stmt_execute(stmt)) {
            auto [code, message] = Error();
            return 0;
        }

        // 受影响的插入行号
        auto result = ::mysql_stmt_insert_id(stmt);
        ::mysql_stmt_close(stmt);
        return result;
    }

    auto Insert(std::string &sql, const std::vector<std::string> &args) -> std::uint64_t {
        if (sql.empty()) {
            return 0;
        }

        std::string strSQL(std::move(sql));
        for (const auto &it : args) {
            boost::algorithm::replace_first(strSQL, "?", fmt::format("'{}'", it));
        }

        if (0 != ::mysql_real_query(mysql_, strSQL.data(), strSQL.size())) {
            return 0;
        }
        return ::mysql_insert_id(mysql_);
    }

    /*
     *  返回受到影响的行
     *  返回0则出错，非0则成功
     */
    auto Insert(const std::string_view sql, const std::vector<std::string> &args) -> std::uint64_t {
        ::MYSQL_STMT *stmt = ::mysql_stmt_init(mysql_);
        if (stmt == nullptr) {
            return 0;
        }

        if (0 != ::mysql_stmt_prepare(stmt, sql.data(), sql.size())) {
            auto [code, message] = Error();
            return 0;
        }

        auto paramCount = ::mysql_stmt_param_count(stmt);

        if (args.size() != paramCount) {
            return 0;
        }

        std::vector<::MYSQL_BIND> vBinds;
        vBinds.reserve(paramCount);

        for (const auto &i : args) {
            ::MYSQL_BIND bind;
            bind.buffer_type   = MYSQL_TYPE_STRING;  //设置占位符的属性
            bind.buffer        = const_cast<char *>(i.data());
            bind.buffer_length = i.size();
            vBinds.emplace_back(bind);
        }

        if (::mysql_stmt_bind_param(stmt, vBinds.data())) {
            auto [code, msg] = Error();
            return 0;
        }

        if (0 != ::mysql_stmt_execute(stmt)) {
            auto [code, msg] = Error();
            return 0;
        }

        // 受影响的插入行号
        auto result = ::mysql_stmt_insert_id(stmt);
        ::mysql_stmt_close(stmt);
        return result;
    }

    // 执行SQL语句，返回影响到的行数
    auto Execute(const std::string_view sql, const std::vector<std::string_view> &args = {}) -> std::uint64_t {
        if (sql.empty()) {
            return 0;
        }

        auto *stmt = ::mysql_stmt_init(mysql_);
        if (stmt == nullptr) {
            return 0;
        }

        if (0 != ::mysql_stmt_prepare(stmt, sql.data(), sql.size())) {
            return 0;
        }

        auto paramCount = ::mysql_stmt_param_count(stmt);

        if (paramCount != args.size()) {
            return 0;
        }

        if (paramCount > 0) {
            std::vector<::MYSQL_BIND> vBinds;
        }

        return 0;
    }

    auto Query(std::vector<std::map<std::string, std::string>> &result, const std::string_view sql) -> bool {
        if (sql.empty()) {
            return false;
        }
        result.clear();
        if (0 != ::mysql_real_query(mysql_, sql.data(), sql.size())) {
            return false;
        }

        auto *tResult = ::mysql_store_result(mysql_);
        if (tResult == nullptr) {
            return false;
        }

        // 行数
        auto numRows = ::mysql_num_rows(tResult);
        if (numRows > 0) {
            result.reserve(numRows);
        } else {
            return false;
        }

        // 数据表字段名
        auto *fields = ::mysql_fetch_fields(tResult);
        if (fields == nullptr) {
            return false;
        }

        // 列数
        auto                               numFields = ::mysql_num_fields(tResult);

        // 字段数据
        std::map<std::string, std::string> element;
        while (auto *row = ::mysql_fetch_row(tResult)) {
            element.clear();
            for (unsigned int i = 0; i < numFields; i++) {
                std::string key   = fields[i].name;
                std::string value = row[i];
                element.emplace_hint(element.end(), key, row[i]);
            }
            result.emplace_back(element);
        }

        ::mysql_free_result(tResult);
        return true;
    }

    auto QueryForMap(std::map<std::string, std::string> &result,
                     const std::string_view              sql,
                     const std::vector<std::string>     &args = {}) -> bool {
        if (sql.empty()) {
            return false;
        }
        result.clear();
        ::MYSQL_STMT *stmt = ::mysql_stmt_init(mysql_);
        if (stmt == nullptr) {
            return false;
        }

        if (0 != ::mysql_stmt_prepare(stmt, sql.data(), sql.size())) {
            return false;
        }

        auto paramCount = ::mysql_stmt_param_count(stmt);

        if (args.size() != paramCount) {
            return false;
        }

        std::vector<::MYSQL_BIND> vBinds;
        if (paramCount > 0) {
            vBinds.reserve(paramCount);
        }

        for (const auto &i : args) {
            ::MYSQL_BIND iBind;
            iBind.buffer_type   = ::MYSQL_TYPE_STRING;  // 设置占位符的属性
            iBind.buffer        = (char *)i.data();
            iBind.buffer_length = i.size();
            vBinds.push_back(iBind);
        }

        if (::mysql_stmt_bind_result(stmt, vBinds.data())) {
            return false;
        }

        if (0 != ::mysql_stmt_execute(stmt)) {
            auto info = Error();
            return false;
        }

        if (0 != ::mysql_stmt_store_result(stmt)) {
            return false;
        }

        while (::mysql_stmt_fetch(stmt) == 0) {}

        ::mysql_stmt_free_result(stmt);
        ::mysql_stmt_close(stmt);
        return true;
    }

    auto QueryForMapSlice(std::vector<std::map<std::string, std::string>> &result,
                          std::string_view                                 sql,
                          const std::vector<std::string>                  &args) -> bool {
        return false;
    }

    auto Shutdown() -> int {
        return ::mysql_shutdown(mysql_, SHUTDOWN_DEFAULT);
    }

    // 关闭连接并释放内存
    void Close() {
        if (!mysql_) {
            ::mysql_close(mysql_);
            mysql_ = nullptr;
        }
    }

private:
    ::MYSQL *mysql_{};
};
}  // namespace uranus::database

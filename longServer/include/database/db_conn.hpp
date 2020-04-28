#pragma once

#include "dsn.hpp"
#include <any>
#include <boost/algorithm/string/replace.hpp>
#include <cstdint>
#include <cstring>
#include <memory>
#include <string>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <typeinfo>
#include <utility>

#ifdef __APPLE__
#include <mariadb/mysql.h>
#else
#include <mysql/mysql.h>
#endif

namespace Uranus::Database
{
class DBConn
{
public:
    DBConn() : mysql(nullptr) {}

    ~DBConn() { Close(); }

    bool Connect(const std::string_view host,
                 const std::string_view user,
                 const std::string_view pwd,
                 const std::string_view db,
                 const std::uint32_t port = 3306)
    {
        if (host.empty() || user.empty()) {
            return false;
        }
        mysql = ::mysql_init(nullptr);
        if (!mysql)
            return false;

        char reconnect = 1;
        // 启用重新连接，必须在调用mysql_real_connect前设置
        if (0 != ::mysql_optionsv(mysql, MYSQL_OPT_RECONNECT, (void *)&reconnect))
            return false;

        if (0 != ::mysql_optionsv(mysql, MYSQL_SET_CHARSET_NAME, (void *)"utf8mb4"))
            return false;

        mysql = ::mysql_real_connect(mysql, host.data(), user.data(), pwd.data(), db.data(), port, nullptr, 0);
        return mysql != nullptr;
    }

    // 重连
    bool Reconnect()
    {
        // 调用该接口必须在mysql_init后设置MYSQL_OPT_RECONNECT
        return int(0) == ::mariadb_reconnect(mysql);
    }

    // error information
    std::tuple<std::uint32_t, std::string> Error() { return {::mysql_errno(mysql), ::mysql_error(mysql)}; }

    // ping
    bool Ping() { return ::mysql_ping(mysql) == 0; }

    bool Timeout(std::uint32_t timeout)
    {
        if (timeout <= 0)
            return false;
        return ::mysql_optionsv(mysql, MYSQL_OPT_CONNECT_TIMEOUT, (void *)&timeout) == 0;
    }

    std::uint64_t Insert(const std::string_view sql, const std::vector<std::any> &args)
    {
        if (sql.empty())
            return 0;

        ::MYSQL_STMT *stmt = ::mysql_stmt_init(mysql);
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
                auto tmp           = std::any_cast<const char *>(it);
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

        if (0 != ::mysql_stmt_bind_param(stmt, vBinds.data())) {
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

    std::uint64_t Insert(std::string &sql, const std::vector<std::string> &args)
    {
        if (sql.empty()) {
            return 0;
        }

        std::string strSQL(std::move(sql));
        for (const auto &it : args) {
            boost::algorithm::replace_first(strSQL, "?", fmt::format("'{}'", it));
        }

        if (0 != ::mysql_real_query(mysql, strSQL.data(), strSQL.size())) {
            return 0;
        }
        return ::mysql_insert_id(mysql);
    }

    /*
     *  返回受到影响的行
     *  返回0则出错，非0则成功
     */
    std::uint64_t Insert(const std::string_view sql, const std::vector<std::string> &args)
    {
        ::MYSQL_STMT *stmt = ::mysql_stmt_init(mysql);
        if (!stmt) {
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

        if (0 != ::mysql_stmt_bind_param(stmt, vBinds.data())) {
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
    std::uint64_t Execute(const std::string_view sql, const std::vector<std::string_view> &args = {})
    {
        if (sql.empty())
            return 0;

        auto stmt = ::mysql_stmt_init(mysql);
        if (!stmt)
            return 0;

        if (0 != ::mysql_stmt_prepare(stmt, sql.data(), sql.size()))
            return 0;

        auto paramCount = ::mysql_stmt_param_count(stmt);

        if (paramCount != args.size())
            return 0;

        if (paramCount > 0) {
            std::vector<::MYSQL_BIND> vBinds;
        }

        return 0;
    }

    bool Query(std::vector<std::map<std::string, std::string>> &result, const std::string_view sql)
    {
        if (sql.empty())
            return false;
        result.clear();
        if (0 != ::mysql_real_query(mysql, sql.data(), sql.size()))
            return false;

        auto tResult = ::mysql_store_result(mysql);
        if (!tResult)
            return false;

        // 行数
        auto numRows = ::mysql_num_rows(tResult);
        if (numRows > 0) {
            result.reserve(numRows);
        } else
            return false;

        // 数据表字段名
        auto fields = ::mysql_fetch_fields(tResult);
        if (!fields)
            return false;

        // 列数
        auto numFields = ::mysql_num_fields(tResult);

        // 字段数据
        std::map<std::string, std::string> element;
        while (auto row = ::mysql_fetch_row(tResult)) {
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

    bool QueryForMap(std::map<std::string, std::string> &result,
                     const std::string_view sql,
                     const std::vector<std::string> &args = {})
    {
        if (sql.empty())
            return false;
        result.clear();
        ::MYSQL_STMT *stmt = ::mysql_stmt_init(mysql);
        if (!stmt) {
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
        if (paramCount > 0)
            vBinds.reserve(paramCount);

        for (const auto &i : args) {
            ::MYSQL_BIND iBind;
            iBind.buffer_type   = ::MYSQL_TYPE_STRING;  // 设置占位符的属性
            iBind.buffer        = (char *)i.data();
            iBind.buffer_length = i.size();
            vBinds.push_back(iBind);
        }

        if (0 != ::mysql_stmt_bind_result(stmt, vBinds.data())) {
            return false;
        }

        if (0 != ::mysql_stmt_execute(stmt)) {
            auto info = Error();
            GLOG_ERRORF("{},{}", std::get<std::string>(info));
            return false;
        }

        if (0 != ::mysql_stmt_store_result(stmt))
            return false;

        while (::mysql_stmt_fetch(stmt) == 0) {
        }

        ::mysql_stmt_free_result(stmt);
        ::mysql_stmt_close(stmt);
        return true;
    }

    bool QueryForMapSlice(std::vector<std::map<std::string, std::string>> &result,
                          const std::string_view sql,
                          const std::vector<std::string> &args)
    {
        return false;
    }

    int Shutdown() { return ::mysql_shutdown(mysql, SHUTDOWN_DEFAULT); }

    // 关闭连接并释放内存
    void Close()
    {
        if (!mysql) {
            ::mysql_close(mysql);
            mysql = nullptr;
        }
    }

private:
    ::MYSQL *mysql;
};
}  // namespace Uranus::Database

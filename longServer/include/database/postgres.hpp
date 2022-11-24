#pragma once

#include <cstdint>
#include <pqxx/pqxx>
#include <string_view>

namespace uranus::database {
class Postgres {
public:
    Postgres()  = default;
    ~Postgres() = default;

    auto Connect(std::string_view dsn) -> bool {
        if (dsn.empty()) {
            return false;
        }
        return false;
    }

    auto Insert(std::string_view query) -> std::int64_t {
        return 0;
    }

private:
    pqxx::connection conn_;
};
}  // namespace uranus::database

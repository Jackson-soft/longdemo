#pragma once

#include <cstdint>
#include <pqxx/connection.hxx>
#include <string_view>

namespace Uranus::Database
{
class Postgres
{
public:
    Postgres()  = default;
    ~Postgres() = default;

    auto connect(std::string_view dsn) -> bool { return false; }

    auto insert(std::string_view query) -> std::int64_t { return 0; }

private:
    pqxx::connection conn;
};
}  // namespace Uranus::Database

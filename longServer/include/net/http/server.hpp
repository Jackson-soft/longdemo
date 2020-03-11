#pragma once

#include "header.hpp"
#include <string>

namespace Uranus
{
namespace Http
{
class Server
{
public:
    Server()  = default;
    ~Server() = default;

public:
    void Close() {}
    void Shutdown() {}
    void Run() {}

private:
    std::string address;
};
}  // namespace Http
}  // namespace Uranus

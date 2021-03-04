#pragma once

#include "header.hpp"
#include <string>

namespace uranus
{
namespace http
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
}  // namespace http
}  // namespace uranus

#pragma once

#include "header.hpp"

#include <string>

namespace uranus::http {
class Server {
public:
    Server()  = default;
    ~Server() = default;

public:
    void close() {}
    void shutdown() {}
    void run() {}

private:
    std::string address_;
};
}  // namespace uranus::http

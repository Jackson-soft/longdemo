#pragma once

#include <arpa/inet.h>
#include <array>
#include <cstring>
#include <netdb.h>
#include <netinet/in.h>
#include <string>
#include <string_view>
#include <sys/socket.h>
#include <utility>
#include <vector>

namespace Uranus::Net
{
class DNS
{
public:
    DNS()  = default;
    ~DNS() = default;

    auto Resolve(std::string_view host) -> bool
    {
        if (host.empty())
            return false;
        struct addrinfo hints {
        };

        struct addrinfo *result{nullptr};

        std::memset(&hints, 0, sizeof(hints));
        hints.ai_family   = PF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_protocol = IPPROTO_TCP;
        hints.ai_flags    = AI_PASSIVE;
        if (::getaddrinfo(host.data(), nullptr, &hints, &result) < 0)
            return false;

        std::array<char, INET6_ADDRSTRLEN> ip{};

        for (auto current = result; current != nullptr; current = current->ai_next) {
            switch (current->ai_family) {
            case PF_INET:
                if (::inet_ntop(current->ai_family,
                                &(((struct sockaddr_in *)(current->ai_addr))->sin_addr),
                                ip.data(),
                                ip.size())
                    == nullptr)
                    continue;
                break;
            case PF_INET6:
                if (::inet_ntop(current->ai_family,
                                &(((struct sockaddr_in6 *)(current->ai_addr))->sin6_addr),
                                ip.data(),
                                ip.size())
                    == nullptr)
                    continue;
                isIPv6 = true;
                break;
            }

            std::string tmp(ip.begin(), ip.end());
            ipList.emplace_back(std::move(tmp));
        }
        ::freeaddrinfo(result);
        return true;
    }

    void LookupHost() {}

    void LookupCNAME() {}

    [[nodiscard]] auto LookupIP() const -> const std::vector<std::string> & { return ipList; }

private:
    std::vector<std::string> ipList;
    bool isIPv6{false};
};
}  // namespace Uranus::Net

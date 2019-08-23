#pragma once

#include <array>
#include <netinet/in.h>
#include <string>
#include <string_view>

namespace Uranus {
// IPV6
class IP {
public:
  IP() = default;
  ~IP() {}

  bool operator=(const IP &lhs, const IP &rhs) { return lhs.mIP == rhs.mIP; }

public:
  // 是否是 IPVe的 ::
  bool IsUnspecified() {
    std::array<std::uint8_t, 16> unspecified{0, 0, 0, 0, 0, 0, 0, 0,
                                             0, 0, 0, 0, 0, 0, 0, 0};
    return mIP == unspecified;
  }

  // 是否是回环地址
  bool IsLoopback() {
    std::array<std::uint8_t, 16> loopback{0, 0, 0, 0, 0, 0, 0, 0,
                                          0, 0, 0, 0, 0, 0, 0, 1};
    return mIP == loopback;
  }

  std::string String() { return ""; }

private:
  void parse(std::string_view str) {
    if (str.empty()) {
      mIP.fill(0);
    } else if (str.size() >= 2 && str.at(0) == ':' && str.at(1) == ':') {
    }
  }

private:
  const uint mLen{16};
  // ipv6 长度是16  std::byte == std::uint8_t
  std::array<std::uint8_t, 16> mIP;
};
} // namespace Uranus
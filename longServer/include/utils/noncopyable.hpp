#pragma once

namespace uranus::utils {
// 工具类
class Noncopyable {
public:
    Noncopyable()  = default;
    ~Noncopyable() = default;

    Noncopyable(const Noncopyable &)                     = delete;
    auto operator=(const Noncopyable &) -> Noncopyable & = delete;
};
}  // namespace uranus::utils

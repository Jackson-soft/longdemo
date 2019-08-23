#pragma once

namespace Uranus
{
// 工具类
class Noncopyable
{
protected:
    Noncopyable()  = default;
    ~Noncopyable() = default;

    Noncopyable(const Noncopyable &) = delete;
    Noncopyable &operator=(const Noncopyable &) = delete;
};
}  // namespace Uranus
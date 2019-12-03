#pragma once

namespace Uranus
{
namespace Utils
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
}  // namespace Utils
}  // namespace Uranus
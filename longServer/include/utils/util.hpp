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

public:
    Noncopyable(const Noncopyable &) = delete;
    auto operator=(const Noncopyable &) -> Noncopyable & = delete;
};
}  // namespace Utils
}  // namespace Uranus

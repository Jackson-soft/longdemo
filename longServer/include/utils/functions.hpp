#pragma once

#include <unistd.h>

namespace Uranus::Utils
{
// 守护进程
inline auto daemon() -> bool { return ::daemon(1, 1) == 0; }
}  // namespace Uranus::Utils
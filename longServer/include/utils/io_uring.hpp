#pragma once

#include "event_loop.hpp"

#include <liburing.h>

namespace uranus::utils {
class IOUring : public EventLoop
{
public:
    IOUring()           = default;
    ~IOUring() override = default;

private:
    struct io_uring_params params_;
    struct io_uring        ring_;
};
}  // namespace uranus::utils
#pragma once

struct noncopyable {
    noncopyable(){};
    noncopyable(const noncopyable &) = delete;
    noncopyable &operator=(const noncopyable &) = delete;
};

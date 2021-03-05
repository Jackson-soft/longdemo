#pragma once

namespace uranus::http
{
enum class Method { Get = 0, Post, Head, Put, Delete, Options, Invalid };

enum class Version { Unknown = 0, http10, http11, http20 };
}  // namespace uranus::http

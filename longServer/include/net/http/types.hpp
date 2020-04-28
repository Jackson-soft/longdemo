#pragma once

namespace Uranus::Http
{
enum class Method { Get = 0, Post, Head, Put, Delete, Options, Invalid };

enum class Version { Unknown = 0, Http10, Http11, Http20 };
}  // namespace Uranus::Http

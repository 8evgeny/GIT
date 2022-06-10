#pragma once
#include <QString>

/** \namespace literal */
inline namespace literal {

constexpr unsigned long long operator "" _b(unsigned long long byte) noexcept {
    return byte;
}
constexpr unsigned long long operator "" _Kb(unsigned long long byte) noexcept {
    return byte * 1024;
}
constexpr unsigned long long operator "" _Mb(unsigned long long byte) noexcept {
    return byte * 1024 * 1024;
}
constexpr unsigned long long operator "" _Gb(unsigned long long byte) noexcept {
    return byte * 1024 * 1024 * 1024;
}
inline QString operator "" _utf8(char const* str, size_t len) {
    return QString::fromUtf8(str, static_cast<int>(len));
}
inline QString operator "" _latin1(char const* str, size_t len) {
    return QString::fromLatin1(str, static_cast<int>(len));
}
inline std::string operator "" _s(char const* str, size_t len) noexcept {
    return std::string(str, len);
}

}  // end namespace literal

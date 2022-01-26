#pragma once
#include <type_traits>

/** \namespace gt::convert */
namespace gt::convert {

/**
 * \code
 *    auto val = std::get<gt::to_utype(UserInfoFields::uiEmail)>(uInfo);
 * \endcode
**/
template<class E>
constexpr auto to_utype(E enumerator) noexcept {
    return static_cast<std::underlying_type_t<E>>(enumerator);
}

} // end namespace gt::convert

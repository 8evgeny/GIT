#pragma once
#include "include/main_pch.h"

/** \namespace convert */
namespace convert {

/**
 * \code
 *    auto val = std::get<convert::to_utype(UserInfoFields::uiEmail)>(uInfo);
 * \endcode
**/
template<class E>
constexpr auto to_utype(E enumerator) noexcept {
    return static_cast<std::underlying_type_t<E>>(enumerator);
}

} // end namespace convert

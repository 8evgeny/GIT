#pragma once
#include <memory>
#include <QLocale>
#include <QString>
#include <QVariant>

/** namespace convert */
namespace convert {
/**
 * \brief Can convert String to unsigned int
 * \code
 *    uint num = convert::toUint(QStringLiteral("4"), 0U);
 * \endcode
**/
template<class T>
inline bool isUint(T&& val) noexcept {
   bool is = false;
   QLocale().toUInt(std::forward<T>(val), &is);
   return is;
}
template<class T>
inline bool isInt(T&& val) noexcept {
   bool is = false;
   QLocale().toInt(std::forward<T>(val), &is);
   return is;
}
/**
 * \brief String to unsigned int
 * \code
 *    uint num = convert::toUint(QStringLiteral("4"), 0U);
 * \endcode
**/
template<class T>
inline unsigned int toUint(T const& val, unsigned int defValue = 0U
        , QLocale const& loc = {}) {
   auto is = false;
   auto const value = loc.toUInt(val, &is);
   return is ? value : defValue;
}
inline unsigned int toUint(QVariant const& val, uint defValue = 0U) {
   return toUint<>(val.toString(), defValue);
}
/**
 * \brief String to unsigned int
 * \code
 *    uint num = convert::toInt(QStringLiteral("4"), INT_MAX);
 * \endcode
**/
template<class T>
inline int toInt(T val, int defValue = 0, QLocale const& loc = {}) {
   auto is = false;
   auto const value = loc.toInt(val, &is);
   return is ? value : defValue;
}
inline int toInt(QVariant&& val, int defValue = 0) {
   return toInt<>(val.toString(), defValue);
}
template<class T>
inline long long toLong(T val, long long defValue = 0, QLocale const loc = {}) {
   auto is = false;
   auto const value = loc.toLongLong(val, &is);
   return is ? value : defValue;
}
inline long long toLong(QVariant&& val, long long defValue = 0) {
   return toLong<>(val.toString(), defValue);
}
} // end namespace convert

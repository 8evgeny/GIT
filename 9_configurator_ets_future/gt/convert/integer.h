/**
 * \file      ibgt/sources/gt/convert/integer.h 
 * \brief     The Integer class provides 
 * \author    S.Panin <dix75@mail.ru>
 * \copyright S.Panin, 2006 - 2020 
 * \version   v.1.0
 * \created   November  (the) 19(th), 2020, 21:10 MSK
 * \updated   November  (the) 19(th), 2020, 21:10 MSK
 * \TODO      
**/
#pragma once
#include <memory>
#include <QLocale>
#include <QString>
#include <QVariant>

/** namespace gt::convert */
namespace gt::convert {

template <class T>
inline auto toNumber(QVariant const& data, T def, bool* res = nullptr) noexcept {
    bool is = false;
    T result{};
    if constexpr(std::is_same_v<T,int>)
        result = data.toInt(&is);
    if constexpr(std::is_same_v<T,unsigned int>)
        result = data.toUInt(&is);
    if constexpr(std::is_same_v<T,double>)
        result = data.toDouble(&is);
    if constexpr(std::is_same_v<T,long>)
        result = data.toLongLong(&is);
    if constexpr(std::is_same_v<T,long long>)
        result = data.toLongLong(&is);
    if constexpr(std::is_same_v<T,unsigned long long>)
        result = data.toULongLong(&is);

    if(res)
        *res = is;
    return is ? result : def;
}

/**
 * \brief Can gt::convert String to unsigned int
 * \code
 *    uint num = gt::convert::toUint(QStringLiteral("4"), 0U);
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
 *    uint num = gt::convert::toUint(QStringLiteral("4"), 0U);
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
 *    uint num = gt::convert::toInt(QStringLiteral("4"), INT_MAX);
 * \endcode
**/
template<class T>
inline int toInt(T val, int defValue = 0, QLocale const& loc = {}) {
   auto is = false;
   auto const value = loc.toInt(val, &is);
   return is ? value : defValue;
}
inline auto toInt(QVariant const& data, int def = {}, bool* res = nullptr) noexcept {
    return toNumber(data, def, res);
}
inline auto toLongLong(QVariant const& data, long long def = {}, bool* res = nullptr) noexcept {
    return toNumber(data, def, res);
}
inline auto toULongLong(QVariant const& data, unsigned long long def = {}, bool* res = nullptr) noexcept {
    return toNumber(data, def, res);
}
template<class T>
inline long long toLong(T val, long long defValue = 0, QLocale const& loc = {}) {
   auto is = false;
   auto const value = loc.toLongLong(val, &is);
   return is ? value : defValue;
}
inline auto toLong(QVariant const& data, long def = {}, bool* res = nullptr) noexcept {
    return toNumber(data, def, res);
}
template<class T>
inline double toDouble(T val, double defValue = 0, QLocale const& loc = {}) noexcept {
   auto is = false;
   auto const value = loc.toDouble(val, &is);
   return is ? value : defValue;
}
inline auto toDouble(QVariant const& data, double def = {}, bool* res = nullptr) noexcept {
    return toNumber(data, def, res);
}
inline auto toBool(QVariant const& data, bool def = {}, bool* res = nullptr) noexcept {
    auto const is = data.isValid();
    if(res) {
        *res = is;
        return data.toBool();
    }
    return def;
}

} // end namespace gt::convert

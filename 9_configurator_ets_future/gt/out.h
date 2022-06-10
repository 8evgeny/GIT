#pragma once
#include <memory>
#include <QDebug>
#include <QString>
#include <type_traits>
#include "convert/enum.h"
#include <iostream>

/** \namespace gt */
namespace gt {

/**
 * \code
    inline std::ostream& operator<<(std::ostream& out, Info const& info) noexcept {
        return gt::cout(out, info);
    }
 * \endcode
**/
template<class T>
static inline std::ostream& cout(std::ostream& out, T const& item) noexcept {
    QString s;
    QDebug(&s) << item;
    out << s.toStdString();
    return out;
}
template<class T>
static inline QString cout(T const& item) noexcept {
    QString s;
    QDebug(&s) << item;
    return s;
}
static inline QString cout(QString const& item) noexcept {
    return item;
}
template<class T, typename = typename std::enable_if<std::is_enum<T>::value, T>::type>
inline QDebug operator<<(QDebug dbg, T type)  {
    return dbg.nospace()
        << "Type: { name: " << toString(type)
        << ", id: " << gt::convert::to_utype(type) << "}";
}
template<class T, typename = typename std::enable_if<std::is_enum<T>::value, T>::type>
inline std::ostream& operator<<(std::ostream& out, T type) noexcept {
    return gt::cout(out, type);
}

} // end namespace gt

template <class T>
inline QDebug operator<<(QDebug debug, std::vector<std::shared_ptr<T>> const& c) noexcept
{
    const bool oldSetting = debug.autoInsertSpaces();
    typename std::vector<std::shared_ptr<T>>::const_iterator it = c.begin(), end = c.end();
    if (it != end) {
        debug << *(*it);
        ++it;
    }
    while (it != end) {
        debug << ", " << *(*it);
        ++it;
    }
    debug << ')';
    debug.setAutoInsertSpaces(oldSetting);
    return debug.maybeSpace();
}

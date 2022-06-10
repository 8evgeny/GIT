#pragma once
#include "double.h"
#include "locale.h"
#include <QString>
#include <QLocale>
#include "include/typedefs.h"

/** \namespace gt */
namespace gt {

/** \namespace empty */
namespace {
inline bool is_char_separator(QChar const& c) noexcept {
    return (c == QChar::fromLatin1(' ') || c == QChar::fromLatin1(',')
         || c == QChar::fromLatin1('.') || c == QChar::Nbsp);
}
} // end namespace empty

inline bool is_double(QString const& str, QLocale const& loc = {}) noexcept {
    bool is = false;
    loc.toDouble(str, &is);
    return is;
}

inline QString to_sqstr(int val, int base = 10) noexcept {
    return QString::number(val, base);
}
inline QString to_sqstr(uint val, int base = 10) noexcept {
    return QString::number(val, base);
}
inline QString to_sqstr(long long val, int base = 10) noexcept {
    return QString::number(val, base);
}
inline QString to_sqstr(uint64_t val, int base = 10) noexcept {
    return QString::number(val, base);
}
inline QString to_sqstr(double val, int precision = 3) noexcept {
    return QString::number(val, 'f', precision);
}
inline QString to_sqstr(QByteArray const& val, QLocale const& = {}) noexcept {
     return QString::fromLatin1(val.constData(), val.size());
}
inline QString to_sqstr(QVariant const& val, QLocale const& = {}) noexcept {
     return val.toString();
}

inline QString to_qstr(QVariant const& var, QLocale const& = {}) noexcept {
    return var.toString();
}
inline QString to_qstr(int val, QLocale const& loc = {}) noexcept {
    return loc.toString(val);
}
inline QString to_qstr(QByteArray const& val, QLocale const& loc = {}) noexcept {
    return to_sqstr(val, loc);
}
inline QString to_qstr(uint val, QLocale const& loc = {}) noexcept {
    return loc.toString(val);
}
inline QString to_qstr(qlonglong val, QLocale const& loc = {}) noexcept {
    return loc.toString(val);
}
inline QString to_qstr(qulonglong val, QLocale const& loc = {}) noexcept {
    return loc.toString(val);
}
inline QString to_qstr(bool val, QLocale const& loc = {}) noexcept {
    return loc.toString(static_cast<int>(val));
}
inline QString to_qstr(double val, QLocale const& loc = {}) noexcept {
    return loc.toString(val);
}
inline QString to_qstr(double val, QLocale const& loc, int precision) noexcept {
    return loc.toString(val, 'f', precision);
}
inline QString to_tr(QVariant const& var, QLocale const& = {}) noexcept {
    return var.toString();
}
//inline QString* to_str(QVariant const& var) noexcept {
//    auto v = var.toString();
//    return &v;
//}

static inline bool from_sqstr(QString const& s, int& val) noexcept {
    bool ok = false;
    val = s.toInt(&ok);
    return ok;
}
static inline bool from_sqstr(QString const& s, double& val) noexcept {
    bool ok = false;
    val = s.toDouble(&ok);
    return ok;
}

static inline bool from_qstr(QString const& str, double& val
        , QLocale const& locale = {}) noexcept {
    bool is = false;
    val = locale.toDouble(str, &is);
    return is;
}
static inline bool from_qstr(QString const& str, int& val
        , QLocale const& locale = {}) noexcept {
    bool is = false;
    val = locale.toInt(str, &is);
    return is;
}

/**
 * \coderead absolute all values independent of locales
**/
static inline bool from_aqstr(QString const& s, int& val
                       , QLocale const& loc = {}) noexcept {
    if(from_qstr(s, val, loc))
        return true;

    auto const& v = gt::locale::groupSeparatorLanguages();
    for(auto i: v)
        if(from_qstr(s, val, i))
            return true;
    return false;
}
static inline bool from_aqstr(QString const& s, double& val
                       , QLocale const& loc = {}) noexcept {
    if(from_qstr(s, val, loc))
        return true;

    auto const& v = gt::locale::doubleLanguages();
    for(auto i: v)
        if(from_qstr(s, val, i))
            return true;
    return false;
}

} // end namespace gt

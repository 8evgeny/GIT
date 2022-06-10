#pragma once
#include <QString>
#include <QRegExp>
#include <QLocale>
#include <QVariant>

/** \namespace gt::convert */
namespace gt::convert {

/**
* \code
*       auto const& str = gt::convert::rstrip(str, QChar('*'));
* \endcode
**/
inline QString rstrip(QString const& str, QChar ch = QChar::fromLatin1(' ')) noexcept {
    auto n = str.size() - 1;

    for (; n >= 0; --n) 
        if (str.at(n) != ch) return str.left(n + 1);
    
    return {};
}
/**
* \code
*      auto const& str = gt::convert::tralling(str, "°C");
* \endcode
**/
inline QString tralling(QString const& str, QString const& end) noexcept {
    return str + end;
}
/**
* \code
*      auto const& str = gt::convert::trallingDegrees(str);
* \endcode
**/
inline QString trallingDegrees(QString const& str) noexcept {
    return tralling(str, QStringLiteral("°C"));
}
inline QString trallingDegrees(int val) noexcept {
    return tralling(QString::number(val), QStringLiteral("°C"));
}
/**
* \code
*      auto const& str = gt::convert::truncate(str, size);
* \endcode
**/
inline QString truncate(QString s, int size) noexcept {
    s.truncate(size);
    return s;
}
/**
* \code
*      auto const& str = gt::convert::truncateComma(str, size);
* \endcode
**/
inline QString truncateComma(QString s, int size) noexcept {
    if (s.size() < size) 
        return s;

    s.truncate(size - 3);
    s += QStringLiteral("...");
    return s;
}
inline auto toPlainText(QString s) noexcept {
#if QT_VERSION > QT_VERSION_CHECK(6, 0, 0)
    s.remove(QRegularExpression(QStringLiteral("<[^>]*>")));
#else
    s.remove(QRegExp(QStringLiteral("<[^>]*>")));
#endif
    return s;
}
inline auto toString(int val, int base = 10) noexcept {
    return QString::number(val, base);
}
inline auto toString(uint val, int base = 10) noexcept {
    return QString::number(val, base);
}
inline auto toString(double val, int precision = 3) noexcept {
    return QString::number(val, 'f', precision);
}
inline auto toString(QVariant const& data, QString const& def = {}, bool* res = nullptr) noexcept {
    auto const is = data.isValid();
    auto const& result = data.toString();
    if(res)
        *res = is;
    return is ? result : def;
}
inline QString toLocalString(int val, QLocale const loc = {}) noexcept {
    return loc.toString(val);
}
inline QString toLocalString(uint val, QLocale const loc = {}) noexcept {
    return loc.toString(val);
}
inline QString toLocalString(double val, QLocale const loc = {}, int precision = 3) noexcept {
    return loc.toString(val, 'f', precision);
}
inline QString capitalize(QString const& str,  QLocale const loc = {}) noexcept {
    if(str.isEmpty())
        return std::move(str);

    auto result = loc.toLower(str);
    result[0] = result[0].toUpper();
    return result;
}
inline QString rightColon(QString const& str) noexcept {
    return str + QChar::fromLatin1(':');
}

/** \namespace html */
namespace html {

inline QString extractBody(QString const& str) noexcept {
    QRegExp rx(QStringLiteral(R"(<body[^>]*>(.*)</body>)"));
    return rx.indexIn(str, 0) == -1 ?
             str : QStringLiteral("<body>%1</body>").arg(rx.cap(1));
}
inline QString extractNoEmptyBody(QString const& str) noexcept {
    auto&& body = extractBody(str);
    auto&& text = toPlainText(body);
    if(text.isEmpty())
        return std::move(text);
#if QT_VERSION > QT_VERSION_CHECK(6, 0, 0)
    return text.indexOf(QRegularExpression(QStringLiteral(R"(\S)"))) == -1
            ? QString()
            : body;
#else
    return text.indexOf(QRegExp(QStringLiteral(R"(\S)"))) == -1
            ? QString()
            : body;
#endif
}

} // end namespace html
} // end namespace gt::convert

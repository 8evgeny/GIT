#pragma once
#include <QString>
#include <QRegExp>
#include <QLocale>
#include <QDateTime>

/** \namespace convert */
namespace convert {

/**
* \code
*       auto const& str = convert::rstrip(str, QChar('*'));
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
*      auto const& str = convert::truncate(str, size);
* \endcode
**/
inline QString truncate(QString s, int size) noexcept {
    s.truncate(size);
    return s;
}
/**
* \code
*      auto const& str = convert::truncateComma(str, size);
* \endcode
**/
inline QString truncateComma(QString s, int size) noexcept {
    if (s.size() < size) 
        return s;

    s.truncate(size - 3);
    s += QStringLiteral("...");
    return s;
}
inline QString toPlainText(QString s) noexcept {
    s.remove(QRegExp(QStringLiteral("<[^>]*>")));
    return s;
}
inline QString toPlainText(QString&& s) noexcept {
    s.remove(QRegExp(QStringLiteral("<[^>]*>")));
    return qMove(s);
}
inline QString toString(int val, int base = 10) noexcept {
    return QString::number(val, base);
}
inline QString toString(uint val, int base = 10) noexcept {
    return QString::number(val, base);
}
inline QString toString(double val, int precision = 3) noexcept {
    return QString::number(val, 'f', precision);
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
inline QString toString(QDateTime const& date) noexcept {
    return date.toString();
}
inline QString toLocalDateTime(QDateTime const& date, QLocale const loc = {}) noexcept {
    return loc.toString(date);
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
    return text.indexOf(QRegExp(QStringLiteral(R"(\S)"))) == -1 ?
             QString() : body;

}

} // end namespace html
} // end namespace convert

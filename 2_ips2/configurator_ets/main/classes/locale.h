#pragma once
#include <QSet>
#include <QLocale>

/** \namespace gt::locale */
namespace gt::locale {

inline QSet<QChar> groupSeparators() Q_DECL_NOEXCEPT {
    return {{
          QChar::fromLatin1('.')
        , QChar::fromLatin1(',')
        , QChar::fromLatin1(' ')
        , QChar(0x066c)
        , QChar(0x2019)
        , QChar::Nbsp
    }};
}
//!< less than groupSeparators because the char 'space'
inline QVector<QLocale::Language> groupSeparatorLanguages() Q_DECL_NOEXCEPT {
    QVector<QLocale::Language> result;
    result.push_back(QLocale::Russian);
    result.push_back(QLocale::English);
    result.push_back(QLocale::German);
    result.push_back(QLocale::CentralKurdish);
    result.push_back(QLocale::SwissGerman);
    return result;
}
inline QSet<QChar> decimalPoints() Q_DECL_NOEXCEPT {
    return {{
        QChar::fromLatin1('.')
      , QChar::fromLatin1(',')
      , QChar(0x066b)
    }};
}
inline QVector<QLocale::Language> decimalPointLanguages() Q_DECL_NOEXCEPT {
    QVector<QLocale::Language> result;
    result.push_back(QLocale::Russian);
    result.push_back(QLocale::English);
    result.push_back(QLocale::Arabic);
    return result;
}
inline QSet<QChar> percents() Q_DECL_NOEXCEPT {
    return {{
        QChar::fromLatin1('%')
      , QChar(0x066a)
    }};
}
inline QVector<QLocale::Language> percentLanguages() Q_DECL_NOEXCEPT {
    QVector<QLocale::Language> result;
    result.push_back(QLocale::Russian);
    result.push_back(QLocale::Pashto);
    return result;
}
inline QVector<QLocale::Language> doubleLanguages() Q_DECL_NOEXCEPT {
    QVector<QLocale::Language> result;
    result.push_back(QLocale::Russian);
    result.push_back(QLocale::English);
    result.push_back(QLocale::German);
    return result;
}

} // end namespace gt::locale

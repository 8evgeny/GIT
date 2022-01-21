#pragma once
#include <array>
#include <QString>
#include <QLocale>
#include <QSettings>
#include <QComboBox>
#include "gt/typedefs.h"
#include "gt/settings/settings.h"

/** begin namespace gt::options */
namespace gt::options {

/**
 * \code
 *    auto&& lang = gt::options::Language::currentBcp47Name();
 *    auto&& lang = gt::options::Language::current();
 *    auto&& langs = gt::options::Language::defaultLanguages();
 *    gt::options::Language::fill(combobox);
 *    auto const result = ui->m_language->findData(lang_id);
 *    m_language = language_t(ui->m_language->currentData().toUInt());
 * \endcode
**/
class Language final {
public:
    using class_name  = Language;
    using id_t        = uint;
    using language_t  = QLocale::Language;
    using languages_t = std::array<language_t,2>;

private:
    language_t m_language = QLocale::AnyLanguage;

private:
    static inline QString path() noexcept {
        return GT_STR("options/lang");
    }
    static inline bool isValidLanguage(language_t l) noexcept {
        return     (l == QLocale::English)
                || (l == QLocale::Russian);
    }
    static inline QString navName(language_t l) noexcept {
        return QLocale(l).nativeLanguageName();
    }
    static inline void addItem(QComboBox* box, language_t l) noexcept {
        box->addItem(navName(l), static_cast<id_t>(l));
    }

public:
    explicit Language() noexcept
        : m_language(current().lang()) {
    }
    Q_DECL_CONSTEXPR explicit Language(language_t l) noexcept
        : m_language(l){
    }
    Q_DECL_CONSTEXPR explicit Language(uint l) noexcept
        : m_language(static_cast<language_t>(l)){
    }
    ~Language() Q_DECL_EQ_DEFAULT;
    friend bool operator==(class_name const& lhs, class_name const& rhs) noexcept {
        return lhs.m_language == rhs.m_language;
    }
    friend bool operator!=(class_name const& lhs, class_name const& rhs) noexcept {
        return !(lhs.m_language == rhs.m_language);
    }
    friend bool operator==(class_name const& lhs, language_t rhs) noexcept {
        return lhs.m_language == rhs;
    }
    friend bool operator!=(class_name const& lhs, language_t rhs) noexcept {
        return !(lhs.m_language == rhs);
    }
    Q_DECL_CONSTEXPR language_t lang() const noexcept {
        return m_language;
    }
    Q_DECL_CONSTEXPR uint toUint() const noexcept {
        return static_cast<uint>(m_language);
    }
    Q_DECL_CONSTEXPR void setLanguage(language_t l) noexcept {
        m_language = l;
    }
    bool isValid() const noexcept {
        return isValidLanguage(m_language);
    }
    QString bcp47Name() const noexcept {
        return QLocale(m_language).bcp47Name();
    }
    static inline QString currentBcp47Name() noexcept {
        return class_name(current()).bcp47Name();
    }
    static inline class_name current() noexcept {
        auto const& val = Settings::val(path(), static_cast<uint>(defaultLanguage().lang()));
        return isValidLanguage(static_cast<QLocale::Language>(val.toUInt()))
                ? class_name(val.toUInt())
                : defaultLanguage();
    }
    static inline bool isCurrentEnglish() noexcept {
        return current().lang() == QLocale::English;
    }
    static inline class_name english() noexcept {
        return class_name{QLocale::English};
    }
    static inline class_name russian() noexcept {
        return class_name{QLocale::Russian};
    }
    static inline void setCurrent(language_t l) noexcept {
        Settings::setVal(path(), static_cast<uint>(l));
    }
    static inline void setCurrent(uint l) noexcept {
        Settings::setVal(path(), l);
    }
    static inline void setCurrent(class_name const& l) noexcept {
        setCurrent(l.lang());
    }
    static inline class_name defaultLanguage() noexcept {
        auto const l = QLocale().language();
        auto const result = isValidLanguage(l) ? l : QLocale::English;
        return class_name(result);
    }
    static inline languages_t defaultLanguages() noexcept {
        return {{
            QLocale::English
          , QLocale::Russian
        }};
    }
    static inline QStringList defaultLanguagesNames() noexcept {
        QStringList result;

        foreach(auto l, defaultLanguages())
            result << navName(l);

        return result;
    }
    static inline void fill(QComboBox* box) noexcept {
        foreach(auto c, defaultLanguages())
            addItem(box, c);
    }
    void swap(Language& rhs) noexcept {
        qSwap(m_language, rhs.m_language);
    }
};
static inline void swap(Language& lhs, Language& rhs) noexcept {
    lhs.swap(rhs);
}

} // end namespace gt::options

inline uint currentLanguageId() noexcept {
    return gt::options::Language::current().toUint();
}
inline uint englishId() noexcept {
    return gt::options::Language::english().toUint();
}
inline uint russianId() noexcept {
    return gt::options::Language::russian().toUint();
}

Q_DECLARE_METATYPE(gt::options::Language)

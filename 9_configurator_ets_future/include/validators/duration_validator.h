#pragma once
#include <QRegExp>
#include <QRegExpValidator>

/** \namespace gt::validator */
namespace gt::validator {

class Duration final: public QRegExpValidator {
public:
    using class_name = Duration;
    using inherited  = QRegExpValidator;

public:
    static inline auto duration() noexcept {
        QRegExp reg(QStringLiteral("^(ONCE|\\d{1,3}s)$"));
        reg.setCaseSensitivity(Qt::CaseInsensitive);
        reg.setPatternSyntax(QRegExp::RegExp);
        return reg;
    }
    static inline auto check(QString const& value, int pos = -1) noexcept {
        auto str = value;
        auto xpos = pos;
        return class_name().validate(str, xpos);
    }

public:
    explicit Duration(QObject* parent = nullptr) noexcept
        : inherited(duration(), parent) {
    }
};
} // end namespace gt::validator

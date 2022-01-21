#pragma once
#include <QRegExp>
#include <QHostAddress>
#include <QRegExpValidator>

class IpValidator final: public QRegExpValidator {
public:
    using class_name = IpValidator;
    using inherited  = QRegExpValidator;

public:
    static inline auto ip() noexcept {
        auto const& range = QStringLiteral("(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])");
        QRegExp reg(QStringLiteral("^%1\\.%1\\.%1\\.%1$").arg(range));
        reg.setCaseSensitivity(Qt::CaseInsensitive);
        reg.setPatternSyntax(QRegExp::RegExp);
        return reg;
    }
    [[deprecated("use isValid")]]
    static inline auto check(QString const& value, int pos = -1) noexcept {
        auto str = value;
        auto xpos = pos;
        return class_name().validate(str, xpos);
    }
    static inline auto isValid(QString const& value) noexcept {
        auto const& host = QHostAddress(value);
        return !host.isNull();
    }
    static inline auto isValid(QVariant const& value) noexcept {
        return isValid(value.toString());
    }

public:
    explicit IpValidator(QObject* parent = nullptr) noexcept
        : inherited(ip(), parent) {
    }
};

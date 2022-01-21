#pragma once
#include <QRegExp>
#include <QRegExpValidator>
#include <QHostAddress>

/** \namespace gt */
namespace gt {

/**
 * \code
    ui->m_last_address->setValidator(new gt::IpValidator(ui->m_last_address));
 * \endcode
 **/
class IpValidator final: public QRegExpValidator {
public:
    using class_name = IpValidator;
    using inherited  = QRegExpValidator;

private:
    static bool checkChar(QChar ch) noexcept {
        return ch.isDigit() || ch == QLatin1Char('.');
    }
    static bool checkStr(QString const& value) noexcept {
        for(auto const ch: value)
            if(!checkChar(ch))
                return false;
        return true;
    }
    static inline auto ip() noexcept {
        auto const& range = QStringLiteral("(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])");
        QRegExp reg(QStringLiteral("^%1\\.%1\\.%1\\.%1$").arg(range));
        reg.setCaseSensitivity(Qt::CaseInsensitive);
        reg.setPatternSyntax(QRegExp::RegExp);
        return reg;
    }

public:
    static inline auto isValid(QString const& value) noexcept {
        if(!checkStr(value))
            return false;

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
    virtual State validate(QString& str, int& pos) const override final {
        if(!checkStr(str))
            return QValidator::Invalid;
        return inherited::validate(str, pos);
    }
};
}  // end namespace gt

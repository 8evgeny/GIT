#pragma once
#include <QRegExp>
#include <QHostAddress>
#include <QRegExpValidator>

class ProjectNumberValidator final: public QRegExpValidator {
public:
    using class_name = ProjectNumberValidator;
    using inherited  = QRegExpValidator;

public:
    static inline auto projectNumber() noexcept {
        QRegExp reg(QStringLiteral("^\\d{3}-\\d{3}-\\d{3}$"));
        reg.setCaseSensitivity(Qt::CaseInsensitive);
        reg.setPatternSyntax(QRegExp::RegExp);
        return reg;
    }

public:
    explicit ProjectNumberValidator(QObject* parent = nullptr) noexcept
        : inherited(projectNumber(), parent) {
    }
};

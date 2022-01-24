#pragma once
#include <QRegExp>
#include <QValidator>
#include "include/typedefs.h"

class SymbolValidator final :public QValidator {
 public:
    using class_name = SymbolValidator;
    using inherited  = QValidator;

 public:
    virtual State validate(QString& str, int&) const override {
        QRegExp rx(IPS_STR(R"(\w{0,5})"));
        rx.setMinimal(false);
        if (rx.exactMatch(str))
            return State::Acceptable;
        return State::Invalid;
    }

 public:
    explicit SymbolValidator(QObject* parent) noexcept
        : inherited(parent)         {
    }
};

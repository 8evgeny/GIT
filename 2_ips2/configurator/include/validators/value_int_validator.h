#pragma once
#include <QValidator>

template <int VALUE = 0>
class ValueIntValidator final : public QValidator {
 public:
    using class_name = ValueIntValidator;
    using inherited  = QValidator;

 public:
    virtual State validate(QString& str, int&) const override {
        return str.toInt() != VALUE
            ? State::Acceptable
            : State::Invalid;
    }

 public:
    explicit ValueIntValidator(QObject* parent) noexcept
        : inherited(parent) {
    }
};

#pragma once
#include <QValidator>

template <int LENGTH>
class StringSizeValidator final :public QValidator {
 public:
    using class_name = StringSizeValidator;
    using inherited  = QValidator;

 public:
    virtual State validate(QString& str, int&) const override {
        return str.length() <= LENGTH
            ? State::Acceptable
            : State::Invalid;
    }   

 public:
    explicit StringSizeValidator(QObject* parent) noexcept
        : inherited(parent) {
    }
};

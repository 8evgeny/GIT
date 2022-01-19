#pragma once
#include <QDebug>
#include <QRegExp>
#include <QRegExpValidator>
#include "include/typedefs.h"

class ActivateControlValidator final: public QValidator {
public:
    using class_name = ActivateControlValidator;
    using inherited  = QValidator;

public:
    static inline auto secondMatch(QString& str) noexcept {
        QRegExp rx(IPS_STR(R"(([Cc][Ii][Dd]\d{1,3}\.\d{1,3};))"));
        rx.setMinimal(true);
        int pos = 0;
        int len = 0;
        while ((pos = rx.indexIn(str, pos)) != -1) {
            pos += rx.matchedLength();
            len += rx.cap(1).size();
        }
        return len == str.size()
            ? State::Acceptable
            : State::Intermediate;
    }
    static inline auto firstMatch(QString& str) noexcept {
        QRegExp rx(IPS_STR(R"([CcIiDd\d\.;]*)"));
        return rx.exactMatch(str);
    }

public:
    explicit ActivateControlValidator(QObject* parent = nullptr) noexcept
        : inherited(parent) {
    }
    virtual State validate(QString& str, int&) const final {
        if(!firstMatch(str))
            return State::Invalid;
        return secondMatch(str);
    }
};

#pragma once
#include <QValidator>
#include <QStringList>

class ContainsValidator final : public QValidator {
 public:
    using class_name = ContainsValidator;
    using inherited  = QValidator;

 public:
    virtual State validate(QString& str, int&) const override {
        return m_list.contains(str) || str == IPS_STR("")
            ? State::Acceptable
            : State::Invalid;
    }

 public:
    explicit ContainsValidator(QStringList const& list, QObject* parent = nullptr) noexcept
        : inherited(parent)
        , m_list(list)
    {
    }

 private:
    QStringList m_list = {};
};

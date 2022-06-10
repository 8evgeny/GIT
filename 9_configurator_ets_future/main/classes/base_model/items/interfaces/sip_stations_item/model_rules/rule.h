#pragma once
#include <QString>

class Rule final {

public:
    using class_name = Rule;

public:
    explicit Rule(QString const& rule = {},  QString const& destination = {})
          : m_rule(rule)
          , m_destination(destination)
    {
    }
    virtual ~Rule() = default;
    void setRule(QString const& rule) noexcept {
        m_rule = rule;
    }
    void setDestination(QString const& destination) noexcept {
        m_destination = destination;
    }    
    auto constexpr const& rule() const noexcept {
        return m_rule;
    }
    auto constexpr const& destination() const noexcept {
        return m_destination;
    }    

private:
    QString m_rule;
    QString m_destination;
};

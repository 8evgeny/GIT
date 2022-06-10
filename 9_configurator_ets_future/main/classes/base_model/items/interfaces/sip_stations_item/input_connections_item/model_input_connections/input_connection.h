#pragma once
#include <QString>
#include "main/classes/base_types/function_types.h"

class InputConnection final {
public:
    using class_name = InputConnection;

public:
    explicit InputConnection(uint number = 0, FunctionType const& function = FunctionType::None, QString const& destination = IPS_STR("None"), uint priority = 0)
          : m_number(number)
          , m_function(function)
          , m_destination(destination)
          , m_priority(priority)
    {
    }
    virtual ~InputConnection() = default;
    void setNumber(uint number) noexcept {
        m_number = number;
    }
    auto number() const noexcept {
        return m_number;
    }
    void setFunction(FunctionType function = FunctionType::None) noexcept {
        m_function = function;
    }
    auto function() const noexcept {
        return m_function;
    }    
    void setDestination(QString const& destination) noexcept {
        m_destination = destination;
    }
    auto const& destination() const noexcept {
        return m_destination;
    }
    auto const& comment() const noexcept {
        return m_comment;
    }
    constexpr void setPriority(uint priority) noexcept {
        m_priority = priority;
    }
    constexpr uint priority() const noexcept {
        return m_priority;
    }
    void setComment(QString const& comment) noexcept {
        m_comment = comment;
    }

private:
    uint m_number = {};
    FunctionType m_function;
    QString m_destination;
    uint m_priority;
    QString m_comment = {};
};

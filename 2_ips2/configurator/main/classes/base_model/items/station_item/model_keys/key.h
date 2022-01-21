#pragma once
#include <vector>
#include "pretone.h"
#include "main/classes/app.h"
#include "main/classes/duration.h"
#include "main/classes/base_types/function_types.h"
#include "main/classes/base_model/items/global_parameters_item/global_parameters.h"

class Key final {
 public:
    using class_name = Key;

 public:
    Key()
        : m_id()
        , m_cid()
        , m_name(IPS_STR("NewKey"))
        , m_priority()
        , m_function(FunctionType::None)
        , m_destination(IPS_STR("None"))
        , m_pretoneChecked(false)
        , m_times()
    {
    }
    void setNumberFunction(FunctionType type) noexcept {
        m_numberFunction = priorityFromFucntionType(type);
    }
    auto numberFunction() const noexcept {
        return m_numberFunction;
    }
    void setId(uint id) noexcept {
        m_id = id;
    }    
    auto id() const noexcept {
        return m_id;
    }
    void setCid(uint cid) noexcept {
        m_cid = cid;
    }
    auto cid() const noexcept {
        return m_cid;
    }
    void setName(QString const& name) noexcept {
        m_name = name;
    }
    auto const& name() const noexcept {
        return m_name;
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
    constexpr void setPriority(uint priority) noexcept {
        m_priority = priority;
    }
    constexpr uint priority() const noexcept {
        return m_priority;
    }
    void setPretone(Pretone const& pretone) noexcept {
        m_pretone = pretone;
    }
    void setPretoneId(uint id) noexcept {
        m_pretone.setId(id);
    }
    void setPretoneDuration(duration_t const& duration) noexcept {
        m_pretone.setDuration(duration);
    }
    auto const& pretone() const noexcept {
        return m_pretone;
    }
    auto const& pretoneChecked() const noexcept {
        return m_pretoneChecked;
    }
    void setActivateControlLines(QString const& lines) noexcept {
        m_lines = lines;
    }
    auto const& activateControlLines() const noexcept {
        return m_lines;
    }
    void setPretoneChecked(bool is) noexcept {
        m_pretoneChecked = is;
    }
    static uint priorityFromFucntionType(FunctionType type) noexcept {
        if (type == FunctionType::PA)
            return APP_SETTINGS->publicAddress();
        if (type == FunctionType::Duplex)
            return APP_SETTINGS->duplex();
        if (type == FunctionType::GroupPA)
            return APP_SETTINGS->groupPa();
        if (type == FunctionType::ConferenceStart)
            return APP_SETTINGS->conference();
        if (type == FunctionType::DynamicCcallGroupStart)
            return APP_SETTINGS->dynamicGroupCall();
        if (type == FunctionType::PlayAlarm)
            return APP_SETTINGS->alarm();
        return 0U;
    }
    void changePriority(FunctionType type) noexcept {
        setPriority(priorityFromFucntionType(type));
    }
    constexpr auto const& times() const noexcept {
        return m_times;
    }
    void setTimes(int times) noexcept {
        m_times = times;
    }

private:    
    uint m_id;
    uint m_cid;
    QString m_name;
    uint m_priority;
    uint m_numberFunction;
    FunctionType m_function;
    QString m_destination;    
    Pretone m_pretone;
    QString m_lines;    
    bool m_pretoneChecked;
    int m_times = 0;
};

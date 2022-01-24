#pragma once
#include "main/classes/cid.h"
#include "main/classes/base_types/station_types.h"

class Station {
public:
    using class_name = Station;

public:
    Station(QString const& name = {},
            cid_t cid = {},
            StationType const& type = {},
            QString const& ip = {},
            QString const& mask = {},
            QString const& gateway = {})
            : m_name(name)
            , m_cid(cid)
            , m_type(type)
            , m_ip(ip)
            , m_mask(mask)
            , m_gateway(gateway)
    {
    }
    virtual ~Station() = default;
    void setStationName(QString const& name) noexcept {
        m_name = name;
    }
    constexpr void setCid(cid_t cid) noexcept {
        m_cid = cid;
    }
    constexpr void setType(StationType type) noexcept {
        m_type = type;
    }
    void setIp(QString const& ip) noexcept {
        m_ip = ip;
    }
    void setId(uint const& id) noexcept {
        m_id = id;
    }
    void setStationMask(QString const& mask) noexcept {
        m_mask = mask;
    }
    void setGateway(QString const& gateway) noexcept {
        m_gateway = gateway;
    }
    auto const& stationName() const noexcept {
        return m_name;
    }
    constexpr cid_t cid() const noexcept {
        return m_cid;
    }
    auto type() const noexcept {
        return m_type;
    }
    auto const& ip() const noexcept {
        return m_ip;
    }
    auto const& id() const noexcept {
        return m_id;
    }
    auto const& stationMask() const noexcept {
        return m_mask;
    }
    auto const& gateway() const noexcept {
        return m_gateway;
    }

private:
    QString m_name;
    cid_t m_cid;
    StationType m_type;
    QString m_ip;
    QString m_mask;
    QString m_gateway;
    uint m_id;
};

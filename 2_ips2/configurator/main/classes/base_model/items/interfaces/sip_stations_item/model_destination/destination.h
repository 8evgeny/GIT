#pragma once
#include "main/classes/cid.h"
#include "classes/base_types/protocol_types.h"
#include "classes/base_types/registration_types.h"

class Destination final {

public:
    using class_name = Destination;

public:
    explicit Destination(QString const& name = {},  QString const& ip = IPS_STR("192.168.10.130"), ProtocolType protocol = ProtocolType::UDP, RegistrationType registr = RegistrationType::ON)
          : m_name(name)
          , m_ip(ip)
          , m_protocol(protocol)
          , m_registr(registr)
    {
    }
    virtual ~Destination() = default;
    void setName(QString const& name) noexcept {        
        m_name = name;
    }
    void setIp(QString const& ip) noexcept {
        m_ip = ip;
    }
    void setProtocol(ProtocolType const& protocol) noexcept {
        m_protocol = protocol;
    }
    void setRegistration(RegistrationType const& registr) noexcept {
        m_registr = registr;
    }
    auto constexpr const& name() const noexcept {
        return m_name;
    }
    auto constexpr const& ip() const noexcept {
        return m_ip;
    }    
    auto constexpr const& protocol() const noexcept {
        return m_protocol;
    }
    auto constexpr const& registr() const noexcept {
        return m_registr;
    }

private:
    QString m_name;
    QString m_ip = IPS_STR("192.168.10.130");
    ProtocolType m_protocol = ProtocolType::UDP;
    RegistrationType m_registr = RegistrationType::ON;
};

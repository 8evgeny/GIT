#pragma once
#include <QString>
#include "main/classes/base_model/items/station_item/station_item.h"

class StationProgrammer final {
    Q_DISABLE_COPY(StationProgrammer)

public:
    StationProgrammer(StationItem* StationItem = nullptr);
    ~StationProgrammer() = default;

public:
    QString const& mac() const noexcept;
    bool status() const noexcept;
    QString const& serialNumber() const noexcept;
    QString const& stConnect() const noexcept;
    StationItem* stationItem();
    void setMac(QString const& mac) noexcept;
    void setStatus(bool status) noexcept;
    void setSerialNumber(QString const& version) noexcept;
    void setStConnect(QString const& status) noexcept;

private:    
    QString m_mac = IPS_STR("239.168.10.100");
    QString m_serialNumber = IPS_STR("43598324987129");
    bool m_status = false;
    StationItem* m_stationItem = nullptr;
};

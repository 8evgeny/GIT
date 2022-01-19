#pragma once
#include "main/classes/cid.h"
#include "main/classes/gid.h"
#include "main/classes/base_model/items/station_item/station_item.h"
#include "main/classes/base_model/items/stations_item/model_stations/model_stations.h"

class GroupCall Q_DECL_FINAL {
public:
    using class_name = GroupCall;
    using station_t = StationItem;

public:
    GroupCall(QString const& name = IPS_STR("NameGroupCall"), gid_t const& gid = {})
            : m_gid(gid)
            , m_name(name)
    {
    }
    virtual ~GroupCall() Q_DECL_EQ_DEFAULT;
    void setNameGroupCall(QString const& name) {
        m_name = name;
    }
    void setGid(gid_t const& gid) {
        m_gid = gid;
    }
    QString const& nameGroupCall() const {
        return m_name;
    }
    gid_t const& gid() const {
        return m_gid;
    }
    void setStationList(QList<station_t*> stationList) noexcept {
        m_stationList = stationList;
    }
    QList<station_t*>const& stationList() const noexcept {
        return m_stationList;
    }
    void setRow(int row) noexcept {
        m_currentRow = row;
    }
    int currentRow() noexcept {
        return m_currentRow;
    }

private:
    int m_currentRow = 0;
    gid_t m_gid;
    QString m_name = {};          
    QList<station_t*> m_stationList = {};
};

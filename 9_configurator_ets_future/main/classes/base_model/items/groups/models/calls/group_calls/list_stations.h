#pragma once
#include "include/main_pch.h"
#include "classes/base_model/items/table_models/model_table_groups/model_table_digital_stations/group_digital_stations/group_digital_stations.h"

class ListStations Q_DECL_FINAL {
    friend class GroupsItem;
    friend class ModelTableGroupsCalls;

public:
    using class_name = ListStations;

public:
    ListStations()
    {
    }
    ~ListStations() Q_DECL_EQ_DEFAULT;

public:
    void setListStations(QList<DigitalStation> const& stations) {
        m_stations = stations;
    }
    QList<DigitalStation> const& stations() {
        return m_stations;
    }

private:
    QList<DigitalStation> m_stations = {};
};

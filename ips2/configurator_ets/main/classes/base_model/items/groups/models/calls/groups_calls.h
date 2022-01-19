#pragma once
#include "include/log.h"
#include "base_table.h"
#include "classes/gid.h"
#include "group_calls/group_calls.h"

class ModelGroups final: public model::BaseTable<QList<GroupCall>> {
    Q_OBJECT
    IPS_DISABLE_COPY(ModelGroups)

 public:
    using class_name = ModelGroups;
    using inherited  = model::BaseTable<QList<GroupCall>>;
    using items_t    = QList<GroupCall>;
    using value_t    = GroupCall;
    using station_t  = StationItem;

 public:
    static const int ListCidRole = Qt::UserRole + 1;

 public:
    explicit ModelGroups(QObject* parent = nullptr);
    ~ModelGroups() = default;
    int columnCount(const QModelIndex &parent = {}) const final;
    QVariant data(const QModelIndex &index, int role) const final;
    bool insertRows(int position, int rows, const QModelIndex& index = {}) override;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override final;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const final;        
    virtual QJsonArray serialize() const override final;
    virtual bool deserialize(QJsonArray const&) override final;
    Qt::ItemFlags flags(const QModelIndex &index) const final {
            return (!index.isValid())
                ? Qt::ItemIsEnabled
                : Qt::ItemIsEnabled | Qt::ItemIsEditable;
    }    
    items_t& items() noexcept {
       return inherited::items();
    }

 public:
    GroupCall* defValue(gid_t gid) const noexcept;
    bool setListStation(const QModelIndex &index, const QList<station_t*>& stations) noexcept;
    QList<station_t*>const& stationList(const QModelIndex &index) const noexcept;

 private:
    QJsonArray serializeListStations(GroupCall const& item) const noexcept;   
    QList<station_t*> deserializeListStations(QJsonObject obj) const noexcept;
};

#pragma once
#include "station.h"
#include "main/classes/base_model/items/station_item/station_item.h"

class StationsItem;

class ModelStations final: public QAbstractTableModel {
    Q_OBJECT
    IPS_DISABLE_COPY(ModelStations)

    friend class TestStationItems;
    friend class TestStationItem;

public:
    using class_name = ModelStations;
    using inherited  = QAbstractTableModel;
    using items_t    = QList<StationItem*>;
    using item_t     = typename items_t::value_type;

private:
    signals:
    void removeStation(QString const& hash);

public:
    static const int SettingsRole = Qt::UserRole + 1;

private:
    QList<StationItem*> StationItems() const noexcept;
    QModelIndex stationsItemIndex() const noexcept;
    void updateCid(item_t item) noexcept;

public:
    explicit ModelStations(QObject* parent = nullptr);
    ~ModelStations() = default;
    int rowCount(const QModelIndex& = {}) const final;
    int columnCount(const QModelIndex& parent = {}) const final;
    QVariant data(const QModelIndex& index, int role) const final;
    virtual bool setData(const QModelIndex& index, const QVariant &value, int role = Qt::EditRole) final;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const final;
    virtual QJsonArray serialize() const;
    virtual bool deserialize(QJsonArray const&) final;
    bool insertRows(int position, int rows, const QModelIndex& = {}) final;    
    bool removeRows(int position, int rows, const QModelIndex& = {}) final;
    Qt::ItemFlags flags(const QModelIndex &index) const final;
    bool insertItem(int position, StationItem* item,  QModelIndex const& index = {});
    void updateItem(int row, Station const& val, const QVector<int>& roles = {}) noexcept;
    Station station(int row) const noexcept;
    void refreshStationItemName(int row) const noexcept;

public:
    void setParentStationItems(StationsItem* parent) {
        m_parent = parent;
    }
    StationsItem* parentStationItems() const {
        return m_parent;
    }
    items_t items() const noexcept {
        return StationItems();
    }
    void setModelTones(TracksModel* modelTones) noexcept {
        m_modelTones = modelTones;
    } 

private:        
    TracksModel* m_modelTones = nullptr;
    StationsItem* m_parent = nullptr;    
};

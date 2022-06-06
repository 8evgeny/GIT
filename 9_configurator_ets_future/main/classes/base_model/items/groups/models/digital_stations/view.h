#pragma once
#include "base_table.h"
#include "digital_stations/digital_stations.h"
#include "main/classes/base_model/items/station_item/station_item.h"

class ModelTableView final : public model::BaseTable<QList<StationItem*>> {
    Q_OBJECT
    IPS_DISABLE_COPY(ModelTableView)

public:
    using class_name = ModelTableView;
    using station_t  = StationItem;
    using inherited  = model::BaseTable<QList<StationItem*>>;

public:
    static const int updateRole = Qt::UserRole + 1;

public:
    explicit ModelTableView(QObject* parent = nullptr);
    ~ModelTableView() = default;    
    int columnCount(const QModelIndex &parent = {}) const final;
    QVariant data(const QModelIndex &index, int role) const  final;    
    QVariant headerData(int section, Qt::Orientation orientation, int role) const  final;    
    virtual QJsonArray serialize() const final {
        return {};
    }
    virtual bool deserialize(QJsonArray const&) final {
        return true;
    }
    Qt::ItemFlags flags(const QModelIndex &index) const  final {
        return (!index.isValid() || index.column() == 0 || index.column() == 1)
            ? Qt::ItemIsEnabled
            : Qt::ItemIsEnabled | Qt::ItemIsEditable;
    }
};

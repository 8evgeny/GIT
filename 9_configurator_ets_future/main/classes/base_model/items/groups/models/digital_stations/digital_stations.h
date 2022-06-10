#pragma once
#include "base_table.h"
#include "digital_stations/digital_stations.h"
#include "classes/base_model/items/stations_item/model_stations/model_stations.h"

class ModelDigitalStations final : public model::BaseTable<QList<DigitalStation>> {
    Q_OBJECT
    IPS_DISABLE_COPY(ModelDigitalStations)

public:
    using class_name = ModelDigitalStations;
    using inherited  = model::BaseTable<QList<DigitalStation>>;

public:
    static const int CidRole = Qt::UserRole + 1;

public:
    explicit ModelDigitalStations(QObject* parent = nullptr);
    ~ModelDigitalStations() = default;
    int columnCount(const QModelIndex &parent = {}) const final;
    QVariant data(const QModelIndex &index, int role) const final;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) final;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const final;
    virtual QJsonArray serialize() const final;
    virtual bool deserialize(QJsonArray const&) final override;
    Qt::ItemFlags flags(const QModelIndex &index) const  final {
        return (!index.isValid() || index.column() == 0 || index.column() == 1)
            ? Qt::ItemIsEnabled
            : Qt::ItemIsEnabled | Qt::ItemIsEditable;
    }
};

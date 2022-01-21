#pragma once
#include <QTableView>
#include "station_programmer/station_programmer.h"

#define IP_STATUS_POSITION 0
#define IP_POSITION 4

class ModelProgrammer final: public model::BaseTable<QList<StationProgrammer*>> {
    Q_OBJECT
    IPS_DISABLE_COPY(ModelProgrammer)

public:
    using inherited  = model::BaseTable<QList<StationProgrammer*>>;
    using class_name = ModelProgrammer;
    using stations_t = QList<StationProgrammer*>;

public:
    static const int StatusRole = Qt::UserRole + 1;

public:
    explicit ModelProgrammer(QObject* parent = nullptr);
    ~ModelProgrammer() = default;
    int rowCount(const QModelIndex& = {}) const override;
    int columnCount(const QModelIndex& parent = {}) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    bool setData(const QModelIndex& index, const QVariant &value, int role = Qt::DisplayRole) final override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    virtual QJsonArray serialize() const override {
        return {};
    }
    virtual bool deserialize(QJsonArray const&) final override {
        return true;
    }
public:
    stations_t& items() noexcept {
        return inherited::items();
    }
};

#pragma once
#include "main/base_table.h"
#include "key.h"

class StationItem;

class ModelKeys final: public model::BaseTable<QList<Key>> {
    Q_OBJECT

 public:
    using class_name = ModelKeys;
    using keys_name  = Key;
    using inherited  = model::BaseTable<QList<Key>>;
    using items_t    = inherited::items_t;
    using value_t    = inherited::value_t;

 public:
    explicit ModelKeys(QObject* parent = nullptr);
    ~ModelKeys() = default;
    int columnCount(const QModelIndex &parent = {}) const override;
    keys_name* defValue(uint id) const noexcept;
    QVariant data(const QModelIndex &index, int role) const override;    
    bool insertRows(int position, int rows, const QModelIndex& index = {}) override;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    virtual QJsonArray serialize() const override;
    virtual bool deserialize(QJsonArray const&) final override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

 public:
    QList<Key> const& keys() const {
         return inherited::items();
    }
    void setParentStationItem(StationItem* parent) {
        m_parent = parent;
    }
    StationItem* parentStationItem() const {
        return m_parent;
    }

 private:
    StationItem* m_parent = nullptr;
};

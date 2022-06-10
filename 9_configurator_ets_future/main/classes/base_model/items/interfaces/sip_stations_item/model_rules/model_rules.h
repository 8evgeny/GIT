#pragma once
#include "rule.h"
#include <gt/model/base_table.h>
#include "main/classes/base_model/items/interfaces/sip_stations_item/model_destination/destination.h"

#define COUNT_COLUMNS_RULES 2

class ModelRules final: public gt::model::BaseTable<QList<Rule>> {
    Q_OBJECT
    IPS_DISABLE_COPY(ModelRules)

public:
    using class_name = ModelRules;
    using inherited  = gt::model::BaseTable<QList<Rule>>;
    using items_t    = inherited::items_t;
    using item_t     = typename items_t::value_type;

public:
    static const int SettingsRole = Qt::UserRole + 1;

public:
    explicit ModelRules(QObject* parent = nullptr);
    ~ModelRules() = default;
    int rowCount(const QModelIndex& = {}) const final;
    int columnCount(const QModelIndex& parent = {}) const final;
    QVariant data(const QModelIndex& index, int role) const final;
    virtual bool setData(const QModelIndex& index, const QVariant &value, int role = Qt::EditRole) final;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const final;
    virtual QJsonArray serialize() const;
    virtual bool deserialize(QJsonArray const&) final;
    bool insertRows(int position, int rows, const QModelIndex& = {}) final;    
    Qt::ItemFlags flags(const QModelIndex &index) const final;

public:
    items_t const& items() const noexcept {
        return inherited::items();
    }
};

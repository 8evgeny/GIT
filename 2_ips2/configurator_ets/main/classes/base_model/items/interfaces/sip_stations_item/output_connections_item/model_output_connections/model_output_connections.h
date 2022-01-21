#pragma once
#include "output_connection.h"
#include "main/base_table.h"
#include "include/typedefs.h"
#include <gt/model/base_table.h>

#define COUNT_COLUMNS_CONNECTIONS 3

class ModelOutputConnections final: public gt::model::BaseTable<QList<OutputConnection>> {
    Q_OBJECT
    IPS_DISABLE_COPY(ModelOutputConnections)

public:
    using class_name        = ModelOutputConnections;
    using connection_name   = OutputConnection;
    using inherited         = gt::model::BaseTable<QList<OutputConnection>>;
    using items_t           = inherited::items_t;
    using item_t            = typename items_t::value_type;

public:
    static const int SettingsRole = Qt::UserRole + 1;
    static const int CommentRole = Qt::UserRole + 2;

private:
    connection_name* defValue(uint cid, QString const& name, uint number) noexcept;

public:
    explicit ModelOutputConnections(QObject* parent = nullptr);
    ~ModelOutputConnections() = default;
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
    items_t const& sips() noexcept {
        return inherited::items();
    }

private:
    QList<uint> m_cids = {};
};

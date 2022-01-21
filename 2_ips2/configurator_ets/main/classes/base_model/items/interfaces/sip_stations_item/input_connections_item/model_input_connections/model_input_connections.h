#pragma once
#include "input_connection.h"
#include "main/base_table.h"
#include "include/typedefs.h"

#define COUNT_COLUMNS_INPUT 4

class ModelInputConnections final: public model::BaseTable<QList<InputConnection>> {
    Q_OBJECT
    IPS_DISABLE_COPY(ModelInputConnections)

public:
    using class_name        = ModelInputConnections;
    using connection_name   = InputConnection;
    using inherited         = model::BaseTable<QList<InputConnection>>;
    using items_t           = inherited::items_t;
    using item_t            = typename items_t::value_type;

public:
    static const int SettingsRole = Qt::UserRole + 1;
    static const int CommentRole = Qt::UserRole + 2;

private:
    void updateCid(item_t item) noexcept;
    connection_name* defValue(uint number = 0, FunctionType const& function = FunctionType::None, QString const& destination = IPS_STR("None"), uint priority = 0) noexcept;

public:
    explicit ModelInputConnections(QObject* parent = nullptr);
    ~ModelInputConnections() = default;
    int rowCount(const QModelIndex& = {}) const final;
    int columnCount(const QModelIndex& parent = {}) const final;
    QVariant data(const QModelIndex& index, int role) const final;
    virtual bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) final;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const final;
    virtual QJsonArray serialize() const override;
    virtual bool deserialize(QJsonArray const&) final override;
    bool insertRows(int position, int rows, const QModelIndex& = {}) final;
    bool removeRows(int position, int rows, const QModelIndex& = {}) final;
    Qt::ItemFlags flags(const QModelIndex &index) const final;

public:
    items_t const& sips() noexcept {
        return inherited::items();
    }
};

#pragma once
#include <QAbstractTableModel>
#include <QDebug>

namespace model {

template <class T>
class BaseTable: public QAbstractTableModel {
    friend class StationsTypeItem;

public:
    using class_name = BaseTable;
    using inherited  = QAbstractTableModel;
    using items_t    = T;
    using value_t    = typename items_t::value_type;

public:
    explicit BaseTable(QObject *parent = nullptr)
        : QAbstractTableModel(parent)
        , m_items() {
    }
    ~BaseTable() = default;
    int rowCount(const QModelIndex& = {}) const override {
        return m_items.size();
    }
    Qt::ItemFlags flags(const QModelIndex &index) const override {
        if (!index.isValid())
            return Qt::ItemIsEnabled;

        return (index.column() == 0)
            ? QAbstractTableModel::flags(index)
            : QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
    }
    bool insertRows(int position, int rows, const QModelIndex& = {}) override {
        inherited::beginInsertRows(QModelIndex(), position, position + rows - 1);
        for (auto row = 0; row != rows; ++row)
            m_items.insert(position + row, value_t());
        inherited::endInsertRows();
        return true;
    }
    bool removeRows(int position, int rows, const QModelIndex& = {}) override {
        inherited::beginRemoveRows(QModelIndex(), position, position + rows - 1);
        for (auto row = 0; row != rows; ++row)
            m_items.removeAt(position);
        inherited::endRemoveRows();
        return true;
    }    
    virtual QJsonArray serialize() const = 0;
    virtual bool deserialize(QJsonArray const&) = 0;
    virtual bool insertItems(int position, int rows, value_t const& value) {
        inherited::beginInsertRows(QModelIndex(), position, position + rows - 1);
        for (auto row = 0; row != rows; ++row)
            m_items.insert(position + row, value);
        inherited::endInsertRows();
        return true;
    }
    virtual bool insertItem(int position, value_t const& value) {
        return insertItems(position, 1, value);
    }    
    virtual bool setItems(items_t const& items) {
        inherited::beginInsertRows({}, 0, items.size());
        m_items = items;
        inherited::endInsertRows();
        return true;
    }
    virtual void setItem(int row, value_t const& val, const QVector<int>& roles = {}) noexcept {        
		Q_UNUSED(roles);
        m_items[row] = val;
        emit dataChanged(index(row, 0), index(row, columnCount()), roles);
    }

public:
    items_t const& items() const noexcept {
        return m_items;
    }
    value_t const& item(int pos) const noexcept {
        return m_items[pos];
    }
    items_t& items() noexcept {
        return m_items;
    }
    value_t& item(int pos) noexcept {
        return m_items[pos];
    }

private:
    items_t m_items;
};
} // end namespace model

/**
 * \file      onan-libgt/sources/gt/model/base_table.h 
 * \brief     The Base_table class provides 
 * \author    S.Panin <dix75@mail.ru>
 * \copyright S.Panin, 2006 - 2021 
 * \version   v.1.1
 * \created   March     (the) 27(th), 2021, 11:30 MSK
 * \updated   March     (the) 27(th), 2021, 11:33 MSK
 * \TODO      
**/
#pragma once
#include <QAbstractTableModel> 

/** \namespace gt::model */
namespace gt::model {

template <class T>
class BaseTable: public QAbstractTableModel {
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
    Qt::ItemFlags flags(QModelIndex const& index) const override {
        if (!index.isValid())
            return Qt::ItemIsEnabled;

        return (index.column() == 0)
            ? QAbstractTableModel::flags(index)
            : QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
    }
    bool insertRows(int position, int rows, QModelIndex const& index = {}) override {
        if(rows < 1)
            return false;

        inherited::beginInsertRows(index, position, position + rows - 1);
        for (auto row = 0; row != rows; ++row)
            m_items.insert(position + row, value_t());
        inherited::endInsertRows();
        return true;
    }
    bool removeRows(int position, int rows, QModelIndex const& index = {}) override {
        if(rows < 1)
            return false;

        inherited::beginRemoveRows(index, position, position + rows - 1);
        for (auto row = 0; row != rows; ++row)
            m_items.removeAt(position);
        inherited::endRemoveRows();
        return true;
    }    
    virtual bool insertItems(int position, int rows, value_t const& value, QModelIndex const& index = {}) {
        if(rows < 1)
            return false;

        inherited::beginInsertRows(index, position, position + rows - 1);
        for (auto row = 0; row != rows; ++row)
            m_items.insert(position + row, value);
        inherited::endInsertRows();
        return true;
    }
    virtual bool insertItem(int position, value_t const& value) {
        return insertItems(position, 1, value);
    }
    virtual bool setItems(items_t const& items) {
        removeRows(0, rowCount());

        inherited::beginInsertRows({}, 0, items.size() - 1);
        m_items = items;
        inherited::endInsertRows();
        return true;
    }
    virtual void setItem(int row, value_t const& val, const QVector<int>& roles = {}) noexcept {
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
} // end namespace gt::model

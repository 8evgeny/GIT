#pragma once
#include <QAbstractListModel>
#include "main/classes/base_types/function_types.h"

template<class T>
class TypeListModel final: public QAbstractListModel {
 public:
    using class_name = TypeListModel;
    using inherited  = QAbstractListModel;
    using items_t    = T;
    using value_t    = typename items_t::value_type;
    using size_t     = typename items_t::size_type;

private:
    size_t row(QModelIndex const& indx) const {
        return static_cast<size_t>(indx.row());
    }

public:
    static const int TypeRole = Qt::UserRole + 1;

 public:
    explicit TypeListModel(QObject* parent = nullptr)
        : inherited(parent) {
    }
    QVariant headerData(int, Qt::Orientation orientation, int role) const final {
        return (role == Qt::DisplayRole && orientation == Qt::Horizontal)
            ? m_header
            : QVariant();
    }
    virtual int rowCount(const QModelIndex& = {}) const final {
        return static_cast<int>(m_items.size());
    }
    QVariant data(const QModelIndex &index, int role) const final {
        if (!index.isValid() || row(index) >= m_items.size() || index.row() < 0)
            return QVariant();

        if (role == Qt::DisplayRole) {
            if constexpr(std::is_same_v<T, QStringList>)
                return m_items[row(index)];
            else
                return toString(m_items[row(index)]);
        }
        if (role == TypeRole)
            return QVariant::fromValue(m_items[row(index)]);
        return {};
    }
    virtual bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) final {
        if (!index.isValid() || row(index) >= m_items.size() || index.row() < 0)
            return false;

        if (role == Qt::EditRole || role == TypeRole)
            m_items[row(index)] = value.value<value_t>();
        return true;
    }
    virtual items_t const& items() const noexcept {
        return m_items;
    }
    virtual void setHeader(QString const& header) noexcept {
        m_header = header;
    }
    virtual QString const& header() const noexcept {
        return m_header;
    }
    virtual bool setItems(items_t const& items) noexcept {
        inherited::beginInsertRows({}, 0, static_cast<int>(items.size()) - 1);
        m_items = items;
        inherited::endInsertRows();
        return true;
    }

 private:
    items_t m_items;
    QString m_header;
};

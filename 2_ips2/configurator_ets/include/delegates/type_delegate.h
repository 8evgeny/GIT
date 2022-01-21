#pragma once
#include <QStyledItemDelegate>
#include "classes/combobox_algo.h"

/** \namespace gt */
namespace gt {

/**
  \code
    using transport_t = gt::TypeDelegate<TransportType, AddCameraModel::TransportTypeRole>;
    static transport_t* initTranportTypeDelegate() noexcept {
        auto&& value = {TransportType::UnicastUDP
                      , TransportType::UnicastTCP
                      , TransportType::Multicast};
        return new transport_t(std::move(value), inherited::parent);
    }
        view->setItemDelegateForColumn(4, initTranportTypeDelegate());
  \endcode
**/

template<class T, int Role>
class TypeDelegate: public QStyledItemDelegate {
public:
    using class_name = TypeDelegate<T, Role>;
    using inherited  = QStyledItemDelegate;
    using type_t     = T;
    using list_t     = QList<T>;

private:
    list_t m_list;

public:
    explicit TypeDelegate(list_t&& list, QObject* parent = nullptr)
        :inherited(parent)
        ,  m_list(std::move(list)) {
    }
    explicit TypeDelegate(list_t const& list, QObject* parent = nullptr)
        :inherited(parent)
        ,  m_list(std::move(list)) {
    }
    virtual QWidget* createEditor(QWidget* parent, QStyleOptionViewItem const&, QModelIndex const&) const override {
        auto box = new QComboBox(parent);
        for (auto const& item: m_list)
            box->addItem(toString(static_cast<type_t>(item)), convert::to_utype(item));        
        return box;
    }
    virtual void setEditorData(QWidget* widget, QModelIndex const& index) const final {
        auto const& value = index.model()->data(index, Qt::EditRole).value<type_t>();
        auto box = static_cast<QComboBox*>(widget);
        gt::cb::setCurrent(box, value);
    }
    virtual void setModelData(QWidget* widget, QAbstractItemModel* model, const QModelIndex& index) const  final {
        auto box = static_cast<QComboBox*>(widget);
        auto&& value = QVariant::fromValue(gt::cb::current<type_t>(box));        
        model->setData(index, std::move(value), Role);
    }
    virtual void updateEditorGeometry(QWidget* widget, QStyleOptionViewItem const& option, QModelIndex const&) const  final {
        widget->setGeometry(option.rect);
    }

};
} // end namespace gt

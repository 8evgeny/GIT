#pragma once
#include <QItemDelegate>
#include "main/classes/cid.h"
#include "main/classes/delegates/combobox_algo.h"

/** \namespace gt */
namespace gt {

/**
  \code
        ui->m_table->setItemDelegateForColumn(1, new gt::CidDelegate(App::model()->cids(), ui->m_table));
  \endcode
**/

class CidDelegate: public QItemDelegate {
public:
    using class_name = CidDelegate;
    using inherited  = QItemDelegate;
    using list_t     = cid_list_t;

 private:
    cid_list_t m_list;

 public:
    explicit CidDelegate(cid_list_t const& list, QObject* parent = nullptr)
        : inherited(parent)
        , m_list(list) {
    }
    virtual QWidget* createEditor(QWidget* parent, QStyleOptionViewItem const&, QModelIndex const&) const final {
        auto box = new QComboBox(parent);
        for (auto const cid: m_list)
            box->addItem(cidToString(cid), cid);
        return box;
    }
    virtual void setEditorData(QWidget* widget, QModelIndex const& index) const  final {
        auto const& value = index.model()->data(index, Qt::EditRole).value<cid_t>();
        auto box = static_cast<QComboBox*>(widget);
        gt::cb::setCurrent(box, value);
    }
    virtual void setModelData(QWidget* widget, QAbstractItemModel* model, const QModelIndex& index) const  final {
        auto box = static_cast<QComboBox*>(widget);
        auto&& value = QVariant::fromValue(gt::cb::current<cid_t>(box));
        model->setData(index, std::move(value), Qt::EditRole);
    }
    virtual void updateEditorGeometry(QWidget* widget, QStyleOptionViewItem const& option, QModelIndex const&) const  final {
        widget->setGeometry(option.rect);
    }
};
} // end namespace gt

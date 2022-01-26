#pragma once
#include <QPainter>
#include <QStyledItemDelegate>
#include "include/widgets/edit_line_button.h"
#include "classes/combobox_algo.h"
#include "main/classes/base_model/items/station_item/model_keys/model_keys.h"

/** \namespace gt::delegate */
namespace gt::delegate {

/**
  \code
        return new delegate::EditLineButton(list, inherited::parent);
        view->setItemDelegateForColumn(4, initTranportTypeDelegate());
  \endcode
**/

class EditLineButton: public QStyledItemDelegate {
   Q_OBJECT

public:
    using class_name = EditLineButton;
    using inherited  = QStyledItemDelegate;

public:
    explicit EditLineButton(QStringList const& list, QObject* parent = nullptr) noexcept
        : QStyledItemDelegate(parent)        
        , m_list(list){
    }
    explicit EditLineButton(QStringList&& list, QObject* parent = nullptr) noexcept
        : QStyledItemDelegate(parent)        
        , m_list(std::move(list)) {
    }
    virtual QWidget* createEditor(QWidget* parent, QStyleOptionViewItem const&, QModelIndex const& index) const override {
        auto editor = new widget::EditLineButton(m_list, parent);
        editor->setText(index.data(Qt::EditRole).toString());
        QObject::connect(editor, &widget::EditLineButton::editingFinished, this, &class_name::commitAndCloseEditor);
        return editor;
    }
    virtual void setEditorData(QWidget* widget, QModelIndex const& index) const  final {
        auto editor = static_cast<widget::EditLineButton*>(widget);
        editor->setText(index.data(Qt::EditRole).toString());
    }
    virtual void setModelData(QWidget* widget, QAbstractItemModel* model, const QModelIndex& index) const final {
        auto editor = static_cast<widget::EditLineButton*>(widget);
        model->setData(index, editor->text(), Qt::EditRole);
    }
    virtual void updateEditorGeometry(QWidget* editor, QStyleOptionViewItem const& option, QModelIndex const&) const final {
        editor->setGeometry(option.rect);
    }

 private slots:
    void commitAndCloseEditor() noexcept {
        auto editor = qobject_cast<widget::EditLineButton*>(sender());
        emit commitData(editor);
        emit closeEditor(editor);
    }

 private:    
    QStringList m_list;
};
} // end namespace gt::delegate

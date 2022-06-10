#pragma once
#include <QCheckBox>
#include <QStyledItemDelegate>

/**
 * \brief A delegate that allows the user to change integer values from the model using a checkbox box widget.
 * \code
 *    delegate::CheckBox m_delegate;
 *    ui->m_table->setEditTriggers(QAbstractItemView::DoubleClicked);
 *    ui->m_table->setItemDelegate(&delegate);
 * \endcode
**/

/** \namespace gt::delegate */
namespace gt::delegate {

class CheckBox: public QStyledItemDelegate {
 public:
    using class_name = CheckBox;
    using inherited  = QStyledItemDelegate;

 public:
    using inherited::inherited;
    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem& /*option*/, const QModelIndex& /*index*/) const override {
        auto editor = new QCheckBox(parent);
        return editor;
    }
    void setEditorData(QWidget *editor, const QModelIndex &index) const override {
        auto cb = qobject_cast<QCheckBox *>(editor);
        cb->setChecked(index.data().toBool());
    }
    void setModelData(QWidget *editor, QAbstractItemModel* model, const QModelIndex &index) const override {
        auto cb = static_cast<QCheckBox *>(editor);
        auto const value = (cb->checkState() == Qt::Checked)? 1 : 0;
        model->setData(index, value, Qt::EditRole);
    }
    void updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem &option, const QModelIndex& /*index*/) const override {
        editor->setGeometry(option.rect);
    }
};
} // end namespace gt::delegate



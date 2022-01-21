#pragma once
#include <QWidget>
#include <QComboBox>
#include <QStyledItemDelegate>

/**
 * \brief A delegate that allows the user to change bool values from the model using a combo box widget.
 * \code
 *    delegate::Bool m_delegate;
 *    ui->m_table->setEditTriggers(QAbstractItemView::DoubleClicked);
 *    ui->m_table->setItemDelegate(&delegate);
 * \endcode
**/

/** \namespace gt::delegate */
namespace gt::delegate {

class Bool: public QStyledItemDelegate {
 public:
    using class_name = Bool;
    using inherited  = QStyledItemDelegate;

 public:
    static const uint bool_role = Qt::UserRole +1;              //!< The bool role for 3 column

 public:
    explicit Bool(QObject* parent = nullptr)
        : inherited(parent) {
    }
    virtual ~Bool() override = default;
    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem& /*option*/, const QModelIndex& /*index*/) const override {
        auto box = new QComboBox(parent);
        box->addItem(class_name::tr("false"));
        box->addItem(class_name::tr("true"));
        return box;
    }
    void setEditorData(QWidget* editor, const QModelIndex& index) const override {
        auto const value = index.model()->data(index, bool_role).toBool();
        auto box = static_cast<QComboBox*>(editor);
        box->setCurrentIndex(value == false ? 0 : 1);
    }
    void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const override {
        auto box = static_cast<QComboBox*>(editor);
        auto const value = box->currentIndex() == 1 ? true : false;
        model->setData(index, value, bool_role);
        model->setData(index, value, Qt::EditRole);
    }
    void updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& /*index*/) const override {
        editor->setGeometry(option.rect);
    }
};
} // end namespace gt::delegate


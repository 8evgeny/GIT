#pragma once
#include <QSpinBox>
#include <QStyledItemDelegate>

/**
 * \brief A delegate that allows the user to change integer values from the model using a spinbox box widget.
 * \code
 *    delegate::SpinBox m_delegate;
 *    m_delegate.setData(0, 100);
 *    ui->m_table->setEditTriggers(QAbstractItemView::DoubleClicked);
 *    ui->m_table->setItemDelegate(&delegate);
 * \endcode
**/

/** \namespace gt::delegate */
namespace gt::delegate {

class SpinBox: public QStyledItemDelegate {
 public:
    using class_name = SpinBox;
    using inherited  = QStyledItemDelegate;

 private:
    int m_min = 0;
    int m_max = 100;

 public:
    SpinBox(int min = 0, int max = 100, QObject* parent = nullptr) noexcept
        : inherited(parent)
        , m_min(min)
        , m_max(max) {
    }
    void setData(int min, int max) noexcept {
        m_min = min;
        m_max = max;
    }
    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem& /*option*/, const QModelIndex& /*index*/) const override {
        auto editor = new QSpinBox(parent);
        editor->setFrame(false);
        editor->setMinimum(m_min);
        editor->setMaximum(m_max);
        return editor;
    }
    void setEditorData(QWidget *editor, const QModelIndex &index) const override {
        auto const value = index.model()->data(index, Qt::EditRole).toInt();
        auto spinBox = static_cast<QSpinBox*>(editor);
        spinBox->setValue(value);
    }
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override {
        auto spinBox = static_cast<QSpinBox*>(editor);
        spinBox->interpretText();
        model->setData(index, spinBox->value(), Qt::EditRole);
    }
    void updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem &option, const QModelIndex& /*index*/) const override {
        editor->setGeometry(option.rect);
    }
};
} // end namespace gt::delegate



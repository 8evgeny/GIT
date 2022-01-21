#pragma once
#include "include/main_pch.h"

/**
 * \code
 *    auto delegate = new gt::delegate::ComboBox({IPS_STR("1"), IPS_STR("2")}, ui->m_table);
 *    ui->m_table->setEditTriggers(QAbstractItemView::DoubleClicked);
 *    ui->m_table->setItemDelegate(delegate);
 * \endcode
**/
/** \namespace gt::delegate */
namespace gt::delegate {

class ComboBox: public QItemDelegate {
public:
    using class_name = ComboBox;
    using inherited  = QItemDelegate;

private:
    QStringList m_list;

public:
    explicit ComboBox(QStringList const& list, QObject* parent = nullptr) noexcept
        : QItemDelegate(parent)
        , m_list(list){
    }
    explicit ComboBox(QStringList&& list, QObject* parent = nullptr) noexcept
        : QItemDelegate(parent)
        , m_list(std::move(list)) {
    }
    virtual ~ComboBox() = default;
    QStringList const& list() const noexcept {
        return m_list;
    }
    void setList(QStringList const& list) noexcept {
        m_list = list;
    }
    void setList(QStringList&& list) noexcept {
        m_list = std::move(list);
    }
    QWidget* createEditor(QWidget* parent, QStyleOptionViewItem const&, QModelIndex const&) const noexcept override {
        auto editor = new QComboBox(parent);
        editor->addItems(m_list);
        return editor;
    }
    void setEditorData(QWidget* editor, QModelIndex const& index) const noexcept override {
        auto box = static_cast<QComboBox*>(editor);
        box->setCurrentText(index.data(Qt::EditRole).toString());
    }
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const noexcept override {
        auto comobo_box = static_cast<QComboBox*>(editor);
        model->setData(index, comobo_box->currentText(), Qt::EditRole);
    }
    void updateEditorGeometry(QWidget* editor, QStyleOptionViewItem const& option, QModelIndex const&) const noexcept override {
        editor->setGeometry(option.rect);
    }
};
} // end namespace gt::delegate

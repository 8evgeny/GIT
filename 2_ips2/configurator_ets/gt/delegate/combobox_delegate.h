#pragma once
#include <QComboBox>
#include <QStyledItemDelegate>

/**
 * \brief A delegate that allows the user to change string values from the model using a combo box widget.
 * \code
 *    gt::delegate::ComboBox m_delegate;
 *    m_delegate.setList({{QStringLiteral("hello")}, {QStringLiteral("GIT")}});
 *    ui->m_table->setEditTriggers(QAbstractItemView::DoubleClicked);
 *    ui->m_table->setItemDelegate(&delegate);
 * \endcode
**/
/** \namespace gt::delegate */
namespace gt::delegate {
    
class ComboBox : public QStyledItemDelegate {
 public:
    using class_name = ComboBox;
    using inherited  = QStyledItemDelegate;

 private:
    QStringList m_list;

 public:
    explicit ComboBox(QObject* parent = nullptr) noexcept
        : inherited(parent) {
    }
    explicit ComboBox(QStringList const& list, QObject* parent = nullptr) noexcept
        : inherited(parent)
        , m_list(list){
    }
    QStringList const& list() const noexcept {
        return m_list;
    }
    void setList(QStringList const& list) noexcept {
        m_list = list;
    }
    void setList(QStringList&& list) noexcept {
        m_list = std::move(list);
    }
    QWidget* createEditor(QWidget* parent, QStyleOptionViewItem const& /*option*/, QModelIndex const& /*index*/) const noexcept override {
        auto editor = new QComboBox(parent);
        editor->addItems(m_list);
        return editor;
    }
    void setEditorData(QWidget* editor, QModelIndex const& index) const noexcept override {
        auto box = static_cast<QComboBox*>(editor);
        box->setCurrentText(index.data(Qt::EditRole).toString());
    }
    void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const noexcept override {
        auto comobo_box = static_cast<QComboBox*>(editor);
        model->setData(index, comobo_box->currentText(), Qt::EditRole);
    }
    void updateEditorGeometry(QWidget* editor, QStyleOptionViewItem const& option, QModelIndex const&) const noexcept override {
        editor->setGeometry(option.rect);
    }
};
} // end namespace gt::delegate

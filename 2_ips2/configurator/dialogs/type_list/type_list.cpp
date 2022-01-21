#include "include/view.h"
#include "type_list.h"
#include "ui_type_list.h"
#include "main/classes/base_types/function_types.h"

template <class T>
TypeListDialog<T>::TypeListDialog(QString const& caption, items_t const& items, value_t const& current, QWidget* parent)
    : QDialog(parent, Qt::FramelessWindowHint)
    , ui(new Ui::TypeListDialog)
{
    ui->setupUi(this);        
    ui->l_caption->setText(caption);
    ui->m_view->setModel(&m_model);
    m_model.setItems(items);
    ui->m_view->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->m_view->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->m_view->setEditTriggers(QAbstractItemView::NoEditTriggers);
    selectRow(current);
}
template <class T>
TypeListDialog<T>::~TypeListDialog()
{
    delete ui;
}
template <class T>
void TypeListDialog<T>::selectRow(value_t const& current)
{
    auto const& items = m_model.items();
    auto const& indx = std::find(items.cbegin(), items.cend(), current);
    auto const dis = std::distance(items.cbegin(), indx);
    mh::simple::view::selectAndClickRow(ui->m_view, m_model, static_cast<int>(dis));
}
template <class T>
typename TypeListDialog<T>::value_t TypeListDialog<T>::current() const noexcept
{
    auto indx = ui->m_view->currentIndex();
    return indx.isValid()
        ?  m_model.data(indx, model_t::TypeRole). template value<value_t>()
        : value_t{};
}
template <class T>
QVariant TypeListDialog<T>::currentVariant() const noexcept
{
    auto indx = ui->m_view->currentIndex();
    return indx.isValid()
        ?  m_model.data(indx, model_t::TypeRole)
        : QVariant{};
}

template class TypeListDialog<function_type::items_t>;
template class TypeListDialog<QStringList>;

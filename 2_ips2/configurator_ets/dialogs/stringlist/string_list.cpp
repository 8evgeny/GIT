#include "include/view.h"
#include "string_list.h"
#include "ui_string_list.h"

StringListDialog::StringListDialog(QString const& caption, list_t const& list, QString const& current, QWidget* parent)
    : QDialog(parent, Qt::FramelessWindowHint)
    , ui(new Ui::StringListDialog)
{
    ui->setupUi(this);        
    ui->l_caption->setText(caption);
    ui->m_view->setModel(&m_model);
    m_model.setStringList(list);
    ui->m_view->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->m_view->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->m_view->setEditTriggers(QAbstractItemView::NoEditTriggers);
    selectRow(current);
}
StringListDialog::~StringListDialog()
{
    delete ui;
}
void StringListDialog::selectRow(QString const& current)
{
    auto cx = stringList().indexOf(current);
    mh::simple::view::selectAndClickRow(ui->m_view, m_model, cx);
}
QString StringListDialog::current() noexcept
{
    auto indx = ui->m_view->currentIndex();
    return indx.isValid()
        ? m_model.data(indx).toString()
        : QString();
}

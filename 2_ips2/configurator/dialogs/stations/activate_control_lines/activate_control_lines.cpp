#include "activate_control_lines.h"
#include "ui_activate_control_lines.h"
#include "include/log.h"
#include "include/view.h"

ActivateControlLinesDialog::ActivateControlLinesDialog(QStringList const& list, QWidget* parent)
    : QDialog(parent, Qt::FramelessWindowHint)
    , ui(new Ui::ActivateControlLinesDialog)
    , m_edit(new gt::widget::EditLineButton(list))
{    
    ui->setupUi(this);    
    ui->m_value->setValidator(new QIntValidator(this));
    auto lay = new QHBoxLayout(ui->m_cid);
    lay->setMargin(0);
    lay->addWidget(m_edit);
    ui->l_cid->setBuddy(m_edit);    
}
ActivateControlLinesDialog::~ActivateControlLinesDialog()
{
    delete ui;
}
QString ActivateControlLinesDialog::cid() const noexcept
{
    return m_edit->text();
}
QString ActivateControlLinesDialog::value() const noexcept
{
    return ui->m_value->text();
}

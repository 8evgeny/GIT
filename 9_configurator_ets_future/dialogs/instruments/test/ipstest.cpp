#include "ipstest.h"
#include "ui_ipstest.h"

IpsTestDialog::IpsTestDialog(QWidget *parent)
    : QDialog(parent, Qt::FramelessWindowHint)
    , ui(new Ui::TestDialog)
{
    ui->setupUi(this);    
}
IpsTestDialog::~IpsTestDialog()
{
    delete ui;
}

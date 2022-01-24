#include "rs232.h"
#include "ui_rs232.h"

Rs232Dialog::Rs232Dialog(QWidget *parent)
    : QDialog(parent, Qt::FramelessWindowHint)
    , ui(new Ui::Rs232Dialog)
{
    ui->setupUi(this);
    instanceNetworkList();
    insatnceEnable();
}
Rs232Dialog::~Rs232Dialog()
{
    delete ui;
}
void Rs232Dialog::insatnceEnable()
{
    auto is = true;
    ui->m_com_ports->setDisabled(is);
    ui->b_open->setDisabled(is);
    ui->b_send->setDisabled(is);
    ui->m_settings->setDisabled(is);
}
void Rs232Dialog::instanceNetworkList()
{
    for (auto&& item : QSerialPortInfo::availablePorts()) {
        ui->m_com_ports->addItem(item.portName());
    }
}

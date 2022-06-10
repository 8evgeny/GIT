#include "network.h"
#include "ui_network.h"

NetworkDialog::NetworkDialog(QWidget* parent)
    : QDialog(parent, Qt::FramelessWindowHint)
    , ui(new Ui::NetworkDialog)
{
    ui->setupUi(this);
}
NetworkDialog::~NetworkDialog()
{
    delete ui;
}

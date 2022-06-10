#include "firmware.h"
#include "ui_firmware.h"

FirmwareDialog::FirmwareDialog(QWidget *parent)
    : inherited(parent, Qt::FramelessWindowHint)
    , ui(new Ui::FirmwareDialog)
{
    ui->setupUi(this);
}
FirmwareDialog::~FirmwareDialog()
{
    delete ui;
}

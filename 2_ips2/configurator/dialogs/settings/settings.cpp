#include "settings.h"
#include "ui_settings.h"

SettingsDialog::SettingsDialog(QDialog *parent)
    : QDialog(parent, Qt::FramelessWindowHint)
    , ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
}
SettingsDialog::~SettingsDialog()
{
    delete ui;
}

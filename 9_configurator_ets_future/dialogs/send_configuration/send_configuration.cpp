#include "send_configuration.h"
#include "ui_send_configuration.h"

SendConfigurationDialog::SendConfigurationDialog(QWidget *parent) :
    QDialog(parent, Qt::FramelessWindowHint)
   , m_ui(new Ui::SendConfiguration)
{
    m_ui->setupUi(this);
    QObject::connect(m_ui->b_ok, &QPushButton::clicked, [this] (bool ok) {
        if (!ok)
           this->close();
    });
}
SendConfigurationDialog::~SendConfigurationDialog()
{
    delete m_ui;
}

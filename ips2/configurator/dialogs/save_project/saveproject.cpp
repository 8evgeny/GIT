#include "saveproject.h"
#include "ui_saveproject.h"

SaveProjectDialog::SaveProjectDialog(QWidget *parent) :
    QDialog(parent, Qt::FramelessWindowHint)
   , m_ui(new Ui::SaveProject)
{
    m_ui->setupUi(this);
}
SaveProjectDialog::~SaveProjectDialog()
{
    delete m_ui;
}

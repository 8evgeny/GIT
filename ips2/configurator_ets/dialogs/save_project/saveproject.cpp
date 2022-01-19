#include "saveproject.h"
#include "ui_saveproject.h"

SaveProjectDialog::SaveProjectDialog(QWidget *parent) :
    QDialog(parent, Qt::FramelessWindowHint)
   , m_ui(new Ui::SaveProject)
{
    m_ui->setupUi(this);
	connect(m_ui->b_yes, SIGNAL(clicked()), this, SLOT(b_yes_clicked()));
	connect(m_ui->b_no, SIGNAL(clicked()), this, SLOT(b_no_clicked()));
	connect(m_ui->b_cancel, SIGNAL(clicked()), this, SLOT(b_cancel_clicked()));
}
SaveProjectDialog::~SaveProjectDialog()
{
    delete m_ui;
}

void SaveProjectDialog::b_yes_clicked()
{
	btnRole = QDialogButtonBox::YesRole;
	close();
}

void SaveProjectDialog::b_no_clicked()
{
	btnRole = QDialogButtonBox::NoRole;
	close();
}

void SaveProjectDialog::b_cancel_clicked()
{
	btnRole = QDialogButtonBox::RejectRole;
	close();
}

QDialogButtonBox::ButtonRole SaveProjectDialog::getExecRole()
{
	return btnRole;
}

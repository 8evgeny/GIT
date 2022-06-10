#pragma once
#include <QDialogButtonBox>
#include "saveproject_pch.h"
#include "saveproject_global.h"

IPS_UI(SaveProject)

class SAVEPROJECT_DIALOG_SHARED_EXPORT SaveProjectDialog final: public QDialog {
	Q_OBJECT
     IPS_DISABLE_COPY(SaveProjectDialog)

 public:
    using inherited = QDialog;
    using class_name = SaveProjectDialog;

 public:
    explicit SaveProjectDialog(QWidget *parent = nullptr);
    ~SaveProjectDialog();

 public:
	QDialogButtonBox::ButtonRole getExecRole();

 public slots:
	 void b_yes_clicked();
	 void b_no_clicked();
	 void b_cancel_clicked();

 private:
    Ui::SaveProject* m_ui = nullptr;
	QDialogButtonBox::ButtonRole btnRole;
};

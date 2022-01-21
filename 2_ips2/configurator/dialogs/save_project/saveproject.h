#pragma once
#include "saveproject_pch.h"
#include "saveproject_global.h"

IPS_UI(SaveProject)

class SAVEPROJECT_DIALOG_SHARED_EXPORT SaveProjectDialog final: public QDialog {
     IPS_DISABLE_COPY(SaveProjectDialog)

public:
    explicit SaveProjectDialog(QWidget *parent = nullptr);
    ~SaveProjectDialog();

private:
    Ui::SaveProject* m_ui = nullptr;
};

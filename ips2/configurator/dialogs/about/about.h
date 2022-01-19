#pragma once
#include "include/typedefs.h"
#include "about_pch.h"
#include "about_global.h"

IPS_UI(AboutDialog)

class ABOUT_DIALOG_SHARED_EXPORT AboutDialog: public QDialog {   
    Q_DISABLE_COPY(AboutDialog)

public:
    explicit AboutDialog(QWidget* parent = nullptr);
    ~AboutDialog() override;

private:
    Ui::AboutDialog* ui = nullptr;
};

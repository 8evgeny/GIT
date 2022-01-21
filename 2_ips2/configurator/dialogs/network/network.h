#pragma once
#include "include/typedefs.h"
#include "network_pch.h"
#include "network_global.h"

IPS_UI(NetworkDialog)

class NETWORK_DIALOG_SHARED_EXPORT NetworkDialog: public QDialog {
    IPS_DISABLE_COPY(NetworkDialog)

public:
    explicit NetworkDialog(QWidget* parent = nullptr);
    ~NetworkDialog();

private:
    Ui::NetworkDialog* ui = nullptr;
};

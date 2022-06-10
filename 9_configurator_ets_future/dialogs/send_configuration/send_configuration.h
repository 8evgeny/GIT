#pragma once
#include "send_configuration_pch.h"
#include "send_configuration_global.h"

IPS_UI(SendConfiguration)

class SENDCONFIGURATION_DIALOG_SHARED_EXPORT SendConfigurationDialog final: public QDialog {
      IPS_DISABLE_COPY(SendConfigurationDialog)

public:
    explicit SendConfigurationDialog(QWidget* parent = nullptr);
    ~SendConfigurationDialog();

private:
    Ui::SendConfiguration* m_ui = nullptr;
};

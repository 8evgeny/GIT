#pragma once
#include "include/typedefs.h"
#include "settings_pch.h"
#include "settings_global.h"

IPS_UI(SettingsDialog)

class SETTINGS_DIALOG_SHARED_EXPORT SettingsDialog: public QDialog {
      IPS_DISABLE_COPY(SettingsDialog)

public:
    explicit SettingsDialog(QDialog *parent = nullptr);
    ~SettingsDialog() override;

private:
    Ui::SettingsDialog* ui = nullptr;
};

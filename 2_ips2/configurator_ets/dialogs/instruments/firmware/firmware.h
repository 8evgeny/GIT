#pragma once
#include "include/typedefs.h"
#include "firmware_pch.h"
#include "firmware_global.h"

IPS_UI(FirmwareDialog)

class FIRMWARE_DIALOG_SHARED_EXPORT FirmwareDialog: public QDialog {
    Q_DISABLE_COPY(FirmwareDialog)

 public:
     using class_name = FirmwareDialog;
     using inherited = QDialog;

 public:
    explicit FirmwareDialog(QWidget *parent = nullptr);
    ~FirmwareDialog() override;

 private:
    Ui::FirmwareDialog* ui = nullptr;
};

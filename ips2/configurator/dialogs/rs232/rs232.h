#pragma once
#include "include/typedefs.h"
#include "rs232_pch.h"
#include "rs232_global.h"

IPS_UI(Rs232Dialog)

class RS232_DIALOG_SHARED_EXPORT Rs232Dialog final: public QDialog {
    Q_OBJECT
    IPS_DISABLE_COPY(Rs232Dialog)

 public:
    using inherited = QDialog;
    using class_name = Rs232Dialog;

 public:
    explicit Rs232Dialog(QWidget* parent = nullptr);
    ~Rs232Dialog() override;

 public:
    void instanceNetworkList();
    void insatnceEnable();

 private:
    Ui::Rs232Dialog* ui = nullptr;
};

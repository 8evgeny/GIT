#pragma once
#include "include/typedefs.h"
#include "ipstest_pch.h"
#include "ipstest_global.h"

IPS_UI(TestDialog)

class IPSTEST_DIALOG_SHARED_EXPORT IpsTestDialog: public QDialog {
    Q_OBJECT
    IPS_DISABLE_COPY(IpsTestDialog)

 public:
    using class_name = IpsTestDialog;
    using inherited = QDialog;

public:
    explicit IpsTestDialog(QWidget* parent = nullptr);
    ~IpsTestDialog() override;

private:
    Ui::TestDialog* ui = nullptr;
};

#pragma once
#include "limitrows_pch.h"
#include "limitrows_global.h"

IPS_UI(LimitRows)

class LIMITROWS_DIALOG_SHARED_EXPORT LimitRowsDialog final: public QDialog {
      IPS_DISABLE_COPY(LimitRowsDialog)

public:
    explicit LimitRowsDialog(QWidget* parent = nullptr);
    ~LimitRowsDialog() override;

private:
    Ui::LimitRows* ui = nullptr;
};

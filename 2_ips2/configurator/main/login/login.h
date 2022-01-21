#pragma once
#include "include/main_pch.h"
#include "include/typedefs.h"
#include <memory>

IPS_UI(LoginDialog)

class LoginDialog: public QDialog {
    Q_OBJECT
    Q_DISABLE_COPY(LoginDialog)

 public:
    using class_name = LoginDialog;
    using inherited  = QDialog;

 public:
    explicit LoginDialog(QWidget* parent = nullptr);
    virtual ~LoginDialog() override;

private:
    void initSignals();

 private slots:
    bool onChanged() noexcept;
    void onAccepted() noexcept;

 private:
    std::unique_ptr<Ui::LoginDialog> ui;
    std::unique_ptr<QTimer> m_timer;
};

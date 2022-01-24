#pragma once
#include <QDialog>
#include <QString>

namespace Ui {
class PDKVLoginDialog;
}

class PDKVLoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PDKVLoginDialog(QWidget *parent = nullptr);
    ~PDKVLoginDialog();

private:
    QString m_url = {};
    Ui::PDKVLoginDialog *ui;
};

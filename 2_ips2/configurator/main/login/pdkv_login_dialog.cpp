#include "pdkv_login_dialog.h"
#include "ui_pdkv_login_dialog.h"
#include "include/typedefs.h"
#include <gt/settings/settings.h>
#include "include/network/http_request.h"
#include "classes/app.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QTimer>
#include <QDebug>
#include <QUrl>

PDKVLoginDialog::PDKVLoginDialog(QWidget *parent) :
    QDialog(parent, Qt::FramelessWindowHint),
    ui(new Ui::PDKVLoginDialog)
{
    ui->setupUi(this);
    m_url = IPS_STR("172.17.4.45");
    ui->m_login->setText(IPS_STR("admin"));
    ui->m_password->setMaxLength(100);
    //ui->m_password->setText(IPS_STR("admin"));
    ui->m_password->setText(IPS_STR("$2a$10$DPkEaSZ7v5HC0Jq/lpW6TeCgmP0ZthWfrXHoD5bZlymMf01VNiL0O)"));
    QObject::connect(ui->m_connect, &QPushButton::clicked, [this] (bool) {
        HttpConnect requester;
        requester.connect(m_url, ui->m_login->text(), ui->m_password->text(), [this] (QJsonValue const& data, QString const& err) {
            qDebug() << "__send__err_data = " << err << data;
            ui->m_data->clear();
            if (err.isEmpty())
                ui->m_data->setText(IPS_STR("No Error"));
            else
                ui->m_data->setText(err);
        });
    });
    QJsonDocument const& doc = App::model()->document();
    QByteArray arr = App::model()->compressData(doc);
    auto const& location = IPS_STR("v1/login");
    QObject::connect(ui->m_get, &QPushButton::clicked, [this, location] (bool) {
        HttpConnect::get(location, [this] (QString const& err, QJsonValue const& data) {
            qDebug() << "___err___data___ = " << err << data;
            ui->m_data->clear();
            ui->m_data->setText(err);
        });
    });
    QObject::connect(ui->m_send, &QPushButton::clicked, [this, location, arr] (bool) {
        HttpConnect connect;
        connect.setUrl(m_url);
        connect.post(location, arr, [this] (QString const& err, QJsonValue const& data) {
            qDebug() << "___err___data___ = " << err << data;
            ui->m_data->clear();
            ui->m_data->setText(err);
        });
    });
    QObject::connect(ui->b_exit, &QPushButton::clicked, [this] (bool) {close();});
}
PDKVLoginDialog::~PDKVLoginDialog()
{
    delete ui;
}

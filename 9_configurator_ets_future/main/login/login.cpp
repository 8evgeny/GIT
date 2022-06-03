#include "login.h"
#include <QTimer>
#include "ui_login.h"
#include <gt/settings/settings.h>
#include "include/network/http_request.h"

LoginDialog::LoginDialog(QWidget* parent)
    : QDialog(parent, Qt::FramelessWindowHint)
    , ui(new Ui::LoginDialog)
    , m_timer(new QTimer(parent))
{
    ui->setupUi(this);    
    initSignals();
    ui->l_version->setText(program::fullProductName());
    ui->l_date->setText(QDate::currentDate().toString(Qt::RFC2822Date));
    ui->m_url->setText(IPS_STR("172.17.4.45"));
    ui->m_login->setText(IPS_STR("admin"));
    ui->m_password->setMaxLength(100);
    ui->m_password->setText(IPS_STR("$2a$10$DPkEaSZ7v5HC0Jq/lpW6TeCgmP0ZthWfrXHoD5bZlymMf01VNiL0O)"));
    QObject::connect(m_timer.get(), &QTimer::timeout, [this]() {
        ui->l_time->setText(QTime::currentTime().toString());
    });
    m_timer->start(100);
    showFullScreen();
}
void LoginDialog::onAccepted() noexcept
{
    ui->m_info->setText({});
    if(onChanged())
        return;

    ui->b_login->setEnabled(false);
    QApplication::processEvents();
    auto const& url = ui->m_url->text();
    IPSHttp requester;
    requester.connect(url, ui->m_login->text(), ui->m_password->text(), [this, url](QString const& err) {
        if (!err.isEmpty())
            ui->m_info->setText(err);
        else
            accept();
        ui->b_login->setEnabled(true);
    });
}
bool LoginDialog::onChanged() noexcept
{
    auto const is =
            ui->m_url->text().isEmpty() ||
            ui->m_login->text().isEmpty() ||
            ui->m_password->text().isEmpty();
    ui->b_login->setDisabled(is);
    return is;
}
void LoginDialog::initSignals()
{
    QObject::connect(ui->b_hideWindow, SIGNAL(clicked()), this, SLOT(showMinimized()));
}
LoginDialog::~LoginDialog()
{
}

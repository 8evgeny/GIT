#include "message_box.h"
#include "ui_message_box.h"

IpsMessageBox::IpsMessageBox(QWidget* parent, QPixmap const& icon, QString const& text)
    : QDialog(parent, Qt::Dialog | Qt::FramelessWindowHint)
    , ui(new Ui::IpsMessageBox)
    , m_timer(new QTimer(parent))
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose, true);
    ui->l_text->setText(text);
    ui->l_iconStatus->setPixmap(icon);

    if (parent)
       setGeometry(parent->geometry().right() - width(), parent->geometry().bottom() - height(), width(), height());

    QTimer::singleShot(3000, this, SLOT(close()));
    QObject::connect(m_timer, &QTimer::timeout, this, [this]() {
        if (m_timer->isActive())
            m_timer->stop();
    });
    m_timer->start(2000);
}
IpsMessageBox::~IpsMessageBox()
{
    delete m_timer;
    delete ui;
}
void IpsMessageBox::info(QWidget* parent, QString const& text) noexcept
{
    auto dlg = new IpsMessageBox(parent, QPixmap(IPS_STR(":/svg/status_ok.svg")), text);
    dlg->show();
}
void IpsMessageBox::error(QWidget* parent, QString const& text) noexcept
{
    auto dlg = new IpsMessageBox(parent, QPixmap(IPS_STR(":/svg/status_error.svg")), text);
    dlg->show();
}

#include "ui_comment.h"
#include "comment.h"
#include "classes/defines.h"
#include "classes/json.h"

OutputComment::OutputComment()
    : ui(new Ui::Comment)
{    
}
OutputComment::~OutputComment()
{
    delete ui;
}
void OutputComment::setupUi(QWidget* parent)
{
    ui->setupUi(parent);
    outputCommentItemsSettingsInstance(ui);
    instanceSignals();
    updateUi();
}
void OutputComment::fildChanged() noexcept
{
   emit(textChanged(ui->t_comment->toPlainText()));
}
void OutputComment::instanceSignals() noexcept
{
    QObject::connect(ui->t_comment, SIGNAL(textChanged()), this, SLOT(fildChanged()));
    QObject::connect(ui->m_oid, SIGNAL(textEdited(QString const&)), this, SIGNAL(oidChanged(QString const&)));
    QObject::connect(ui->m_name, SIGNAL(textEdited(QString const&)), this, SIGNAL(nameChanged(QString const&)));
    QObject::connect(ui->m_number, SIGNAL(textEdited(QString const&)), this, SIGNAL(numberChanged(QString const&)));
    QObject::connect(ui->m_oid, SIGNAL(editingFinished()), this, SLOT(editData()));
    QObject::connect(ui->m_name, SIGNAL(editingFinished()), this, SLOT(editData()));
    QObject::connect(ui->m_number, SIGNAL(editingFinished()), this, SLOT(editData()));
}
void OutputComment::editData() noexcept
{
    m_oid = ui->m_oid->text().toUInt();
    m_name = ui->m_name->text();
    m_number = ui->m_number->text().toInt();
}
void OutputComment::updateUi() noexcept
{
    ui->m_oid->setText(QString::number(static_cast<int>(m_oid)));
    ui->m_name->setText(m_name);
    ui->m_number->setText(QString::number(m_number));
    ui->t_comment->setText(m_comment);
}
void OutputComment::setOutputConnection(OutputConnection const& connection) noexcept
{
    m_oid = connection.oid();
    m_name = connection.name();
    m_number = static_cast<int>(connection.number());
    qDebug() << "_____comment_____ = " << connection.comment();
    m_comment = connection.comment();
    updateUi();
}
QJsonValue OutputComment::serialize() const
{
    return QJsonObject {
            {IPS_STR("oid"), static_cast<int>(m_oid)}
           ,{IPS_STR("name"), m_name}
           ,{IPS_STR("number"), m_number}};
}
bool OutputComment::deserialize(QJsonObject const& data)
{
   IPS_JSON_TO_UINT(data, IPS_STR("oid"), oid)
   m_oid = oid;

   IPS_JSON_TO_STR(data, IPS_STR("name"), name)
   m_name = name;

   IPS_JSON_TO_INT(data, IPS_STR("number"), number)
   m_number = number;
   return true;
}
void OutputComment::clear() noexcept
{
    ui->m_oid->clear();
    ui->m_name->clear();
    ui->m_number->clear();
    m_oid = 0;
    m_name.clear();
    m_number = 0;
}

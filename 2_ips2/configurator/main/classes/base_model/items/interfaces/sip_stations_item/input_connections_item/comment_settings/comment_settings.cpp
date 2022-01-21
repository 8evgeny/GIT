#include "comment_settings.h"
#include "ui_comment_settings.h"
#include "classes/defines.h"
#include "classes/json.h"
#include "classes/combobox_algo.h"
#include <QStyledItemDelegate>

CommentSettings::CommentSettings()
    : ui(new Ui::CommentSettings)
{
}
CommentSettings::~CommentSettings()
{
    delete ui;
    ui = nullptr;
}
void CommentSettings::setupUi(QWidget* parent)
{
    ui->setupUi(parent);
    commentItemsSettingsInstance(ui);
    instanceSignals();
    updateUi();

    QStyledItemDelegate* itemDelegate = new QStyledItemDelegate();
    ui->m_function->setItemDelegate(itemDelegate);
}
void CommentSettings::updateUi() noexcept
{
    ui->m_destination->setText(m_destination);
    ui->m_priority->setText(m_priority);
    ui->m_input_number->setText(m_input_number);
    ui->t_comment->setText(m_comment);
}
void CommentSettings::fildChanged() noexcept
{
   emit(textChanged(ui->t_comment->toPlainText()));
}
void CommentSettings::instanceSignals() noexcept
{      
    QObject::connect(ui->t_comment, SIGNAL(textChanged()), this, SLOT(fildChanged()));
    QObject::connect(ui->m_destination, SIGNAL(editingFinished()), this, SLOT(editData()));
    QObject::connect(ui->m_priority, SIGNAL(editingFinished()), this, SLOT(editData()));
    QObject::connect(ui->m_input_number, SIGNAL(editingFinished()), this, SLOT(editData()));
    QObject::connect(ui->m_function, SIGNAL(currentIndexChanged(QString const&)), this, SLOT(editData()));
    QObject::connect(ui->m_function, SIGNAL(currentIndexChanged(QString const&)), this, SIGNAL(functionChanged(QString const&)));
    QObject::connect(ui->m_destination, SIGNAL(textEdited(QString const&)), this, SIGNAL(destinationChanged(QString const&)));
    QObject::connect(ui->m_priority, SIGNAL(textEdited(QString const&)), this, SIGNAL(priorityChanged(QString const&)));
    QObject::connect(ui->m_input_number, SIGNAL(textEdited(QString const&)), this, SIGNAL(inputNumberChanged(QString const&)));    
}
void CommentSettings::editData()
{
    m_function = gt::cb::current<FunctionType>(ui->m_function);
    m_destination = ui->m_destination->text();
    m_priority = ui->m_priority->text();
    m_input_number = ui->m_input_number->text();
    m_comment = ui->t_comment->toPlainText();
}
void CommentSettings::setInputConnection(InputConnection const& connnection) noexcept
{    
    gt::cb::setCurrent(ui->m_function, connnection.function());
    m_function = connnection.function();
    m_destination = connnection.destination();
    m_priority = QString::number(static_cast<int>(connnection.priority()));
    m_input_number = QString::number(static_cast<int>(connnection.number()));
    m_comment = connnection.comment();
    updateUi();
}
QJsonValue CommentSettings::serialize() const
{
    return QJsonObject {
        {IPS_STR("function"), static_cast<int>(m_function)}
       ,{IPS_STR("destination"), m_destination}
       ,{IPS_STR("priority"), m_priority}
       ,{IPS_STR("input_number"), m_input_number}};
}
bool CommentSettings::deserialize(QJsonObject const& data)
{    
    IPS_JSON_TO_INT(data, IPS_STR("function"), function)
    m_function = static_cast<FunctionType>(function);

    IPS_JSON_TO_STR(data, IPS_STR("destination"), destination)
    m_destination = destination;

    IPS_JSON_TO_STR(data, IPS_STR("priority"), priority)
    m_priority = priority;

    IPS_JSON_TO_STR(data, IPS_STR("input_number"), input_number)
    m_input_number = input_number;
    return true;
}
void CommentSettings::clear() noexcept
{
    ui->m_function->clear();
    ui->m_destination->clear();
    ui->m_priority->clear();
    ui->m_input_number->clear();    
    m_destination.clear();
    m_priority.clear();
    m_input_number.clear();
}

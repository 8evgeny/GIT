#include "include/view.h"
#include "classes/json.h"
#include "ui_project_info.h"
#include "project_info_item.h"
#include "project_info_instance.h"
#include "include/convert/string.h"

void ProjectInfoItem::instanceData()
{ 
    ui->m_customer->setMaxLength(256);
    ui->m_customer->setText(customer());
    ui->m_location->setMaxLength(256);
    ui->m_location->setText(location());    
    ui->m_number->setText(projectNumber());
    ui->m_number->setPlaceholderText(IPS_STR("Only numbers"));
    ui->m_id->setValidator(new QIntValidator(0, 1000, ui->m_id));
    ui->m_id->setText(systemID());
    ui->m_id->setPlaceholderText(IPS_STR("Only numbers"));
    ui->m_desc->setText(convert::truncate(description(), 256));

    ui->m_view->setModel(&m_model);
    ui->m_view->setFocusPolicy(Qt::TabFocus);
    ui->m_view->setShowGrid(false);
    ui->m_view->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->m_view->setContextMenuPolicy(Qt::CustomContextMenu);    
    ui->m_view->resizeColumnsToContents();
    ui->m_view->horizontalHeader()->resizeSection(0, 200);
    ui->m_view->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
    ui->m_remove->setEnabled(m_model.rowCount());
}
void ProjectInfoItem::trancateDescription() noexcept
{
    QObject::connect(ui->m_desc, &QTextEdit::textChanged, [this] () {
        auto desc = ui->m_desc->toPlainText();
        if (ui->m_desc->toPlainText().length() > static_cast<int>(m_maxDescription)) {
            desc.truncate(m_maxDescription);
            ui->m_desc->setPlainText(desc);
        }
        setDescription(desc);
    });
}
void ProjectInfoItem::selectedRowDelete() noexcept
{
    auto const& indx = mh::selected(ui->m_view);
    if (!indx.isValid())
        return;

    m_model.removeRow(indx.row());
    mh::simple::view::selectAndClickRow(ui->m_view, m_model, (indx.row() == 0 ? 0 : indx.row()));

    if (m_currentRow >= m_model.rowCount())
        mh::simple::view::selectAndClickRow(ui->m_view, m_model, (indx.row() == 0 ? 0 : indx.row()-1));
}
void ProjectInfoItem::instanceSiganls()
{
    QObject::connect(ui->m_customer, &QLineEdit::textChanged, this, &ProjectInfoItem::setCustomer);
    QObject::connect(ui->m_location, &QLineEdit::textChanged, this, &ProjectInfoItem::setLocation);
    QObject::connect(ui->m_number, &QLineEdit::textChanged, this, &ProjectInfoItem::setProjectNumber);
    QObject::connect(ui->m_id, &QLineEdit::textChanged, this, &ProjectInfoItem::setSystemID);    
    QObject::connect(ui->m_view, &QTableView::customContextMenuRequested, this, [this] (const QPoint &) {
       QMenu menu;
       menu.addAction(QIcon(IPS_STR(":/svg/plus_qmenu_dark.svg")), iMenu::tr("Add comment"), [this] (){
           ui->m_add->clicked();
       });
       auto rem = menu.addAction(QIcon(IPS_STR(":/svg/minus_qmenu_dark.svg")), iMenu::tr("Remove comment"), [this] (){
           ui->m_remove->clicked();
       });
       menu.addSeparator();
       auto remAll = menu.addAction(QIcon(IPS_STR(":/svg/basket_qmenu_dark.svg")), iMenu::tr("Remove all comments"), [this ] () {
           m_model.removeRows(0, m_model.rowCount());
           ui->m_remove->setEnabled(m_model.rowCount());
       });
       remAll->setEnabled(m_model.rowCount());
       rem->setEnabled(m_model.rowCount());
       menu.exec(QCursor::pos());
    });
    QObject::connect(ui->m_add, &QToolButton::clicked, this, [this]() {
       m_model.insertRows(m_model.rowCount({}), 1);
       ui->m_remove->setEnabled(true);
       mh::simple::view::selectRow(ui->m_view, m_model.items().size()-1);
    });
    QObject::connect(ui->m_view, &QTableView::clicked, this, [this](const QModelIndex& index) {
       mh::simple::view::selectRow(ui->m_view, index.row());
       m_currentRow = index.row();
    });
    QObject::connect(ui->m_remove, &QToolButton::clicked, [this] () {
       selectedRowDelete();
       ui->m_remove->setEnabled(m_model.rowCount());
    });
}
void ProjectInfoItem::changeEvent(QEvent* event)
{
    IPS_LANGUGE_EVENT(ui, this);
    QWidget::changeEvent(event);
}
ProjectInfoItem::~ProjectInfoItem()
{
    deshow();
}
ProjectInfoItem* ProjectInfoItem::show(QWidget* content, QWidget* settings)
{
    ips::algo::removeChildren(content);
    ips::algo::removeChildren(settings);
    ips::algo::createContent(ui, content);
    instanceData();
    instanceSiganls();
    trancateDescription();
    ProjectInfoInstance(ui);
    mh::simple::view::selectFirstRow(ui->m_view, m_model);
    return this;
}
void ProjectInfoItem::deshow()
{
    delete ui;
    ui = nullptr;
}
QString ProjectInfoItem::name(BaseItem*) const
{
    return iInfo::tr("Project information");
}
QJsonValue ProjectInfoItem::serialize() const
{
    return QJsonObject {
            {IPS_STR("customer"), m_customer}
          , {IPS_STR("location"), m_location}
          , {IPS_STR("number"), m_number}
          , {IPS_STR("id"), m_id}
          , {IPS_STR("desc"), m_desc}
          , {IPS_STR("comments"), m_model.serialize()}
    };
}
bool ProjectInfoItem::clear()
{
    m_customer.clear();
    m_location.clear();
    m_number.clear();
    m_id.clear();
    m_desc.clear();
    if (m_model.rowCount())
        m_model.removeRows(0, m_model.rowCount());
    return true;
}
bool ProjectInfoItem::deserialize(QJsonObject const& data)
{
    IPS_JSON_EXTRACT_OBJECT(data, info, IPS_STR("Project information"))
    IPS_JSON_TO_SIMPLE_STR(info.toObject(), IPS_STR("customer"), m_customer)
    IPS_JSON_TO_SIMPLE_STR(info.toObject(), IPS_STR("location"), m_location)
    IPS_JSON_TO_SIMPLE_STR(info.toObject(), IPS_STR("number"), m_number)
    IPS_JSON_TO_SIMPLE_STR(info.toObject(), IPS_STR("id"), m_id)
    IPS_JSON_TO_SIMPLE_STR(info.toObject(), IPS_STR("desc"), m_desc)

    IPS_JSON_EXTRACT_ARRAY(info.toObject(), arr, IPS_STR("comments"))
    return m_model.deserialize(arr.toArray());
}

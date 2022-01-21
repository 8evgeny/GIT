#include "include/view.h"
#include "classes/algo.h"
#include "classes/app.h"
#include "classes/defines.h"
#include "ui_output_connections_item.h"
#include "output_connections_item.h"
#include "output_connections_instance.h"
#include "include/tr.h"
#include <QDebug>

OutputConnectionsItem::OutputConnectionsItem(BaseItem* item)
     : BaseItem(item)
{
}
OutputConnectionsItem::~OutputConnectionsItem()
{
    deshow();
}
void OutputConnectionsItem::changeEvent(QEvent* event)
{
    if (event->type() == QEvent::LanguageChange) {
        if (ui)
            ui->retranslateUi(this);        
    }
    QWidget::changeEvent(event);
}
OutputConnectionsItem* OutputConnectionsItem::show(QWidget* content, QWidget* settings) {
    ips::algo::createContent(ui, content);
    ips::algo::removeChildren(settings);
    outputConnectionsItemInstance(ui, &m_model);
    mh::simple::view::selectAndClickRow(ui->m_table_output, m_model, m_model.rowCount()-1);
    App::model()->settings()->setVisible(m_model.rowCount());
    m_comment.setupUi(settings);
    instanceSignals();
    ui->m_rem->setEnabled(m_model.rowCount());
    return this;
}
void OutputConnectionsItem::deshow()
{
    delete ui;
    ui = nullptr;
}
bool OutputConnectionsItem::error(BaseItem*) const
{
    return true;
}
QString OutputConnectionsItem::name(BaseItem*) const
{    
    return iOtputConnections::tr("Output connections");
}
QJsonValue OutputConnectionsItem::serialize() const
{
    return QJsonObject {
        {IPS_STR("connection_settings"), m_comment.serialize()}
        , {IPS_STR("connections"), m_model.serialize()}};
}
bool OutputConnectionsItem::clear()
{    
    ips::algo::clear(&m_model);
    return true;
}
bool OutputConnectionsItem::deserialize(QJsonObject const& data)
{       
    IPS_JSON_EXTRACT_OBJECT(data, item, IPS_STR("OutputConnections"))
    IPS_JSON_EXTRACT_OBJECT(item.toObject(), settings, IPS_STR("connection_settings"))
    IPS_JSON_EXTRACT_ARRAY(item.toObject(), connections, IPS_STR("connections"))
    m_comment.deserialize(settings.toObject());
    m_model.deserialize(connections.toArray());
    return true;
}
void OutputConnectionsItem::instanceSignals() noexcept
{
    QObject::connect(&m_comment, &OutputComment::textChanged, [this] (QString const& text) {
        if (text.isEmpty())
            return;

        m_model.setData(m_index, text, ModelOutputConnections::CommentRole);
    });
    QObject::connect(ui->m_table_output, &QTableView::clicked, [this] (const QModelIndex& index) {
        if (!index.isValid())
            return;

        m_currentRow = index.row();
        m_index = index;
        m_row = index.row();
        auto connection = m_model.item(m_row);
        m_comment.setOutputConnection(connection);
        m_comment.updateUi();
        mh::simple::view::selectRow(ui->m_table_output, index.row());
    });
    QObject::connect(&m_model, &ModelOutputConnections::dataChanged, [this] (const auto& index, const auto&, auto const& roles) {
        if (index.isValid() && roles.size() == 1 && roles[0] == Qt::EditRole)
            m_comment.setOutputConnection((m_model.item(index.row())));
    });
    QObject::connect(&m_comment, &OutputComment::oidChanged, this, [this] (QString const& oid) {
        auto index =  mh::selected(ui->m_table_output);
        if (index.isValid()) {
            QModelIndex ind = m_model.index(index.row(), CID_COLUMN, index);
            m_model.setData(ind, oid);
        }
    });
    QObject::connect(&m_comment, &OutputComment::nameChanged, this, [this] (QString const& name) {
        auto index = mh::selected(ui->m_table_output);
        if (index.isValid()) {
            QModelIndex ind = m_model.index(index.row(), NAME_COLUMN, index);
            m_model.setData(ind, name);
        }
    });
    QObject::connect(&m_comment, &OutputComment::numberChanged, this, [this] (QString const& number) {
        auto index =  mh::selected(ui->m_table_output);
        if (index.isValid()) {
            QModelIndex ind = m_model.index(index.row(), NUMBER_COLUMN, index);
            m_model.setData(ind, number);
        }
    });    
    QObject::connect(ui->m_add, &QPushButton::clicked, [this] (bool) {
        m_model.insertRow(m_model.rowCount());
        ui->m_rem->setEnabled(m_model.rowCount());
        App::model()->settings()->setVisible(true);
        mh::simple::view::selectAndClickRow(ui->m_table_output, m_model, m_model.rowCount()-1);
    });
    QObject::connect(ui->m_rem, &QToolButton::clicked, [this] () {
    ips::algo::selectedRowDelete<ModelOutputConnections, QTableView>(&m_model, ui->m_table_output, m_currentRow);
    if (!m_model.rowCount()) {
        ui->m_rem->setDisabled(true);
        App::model()->settings()->setVisible(false);
        mh::simple::view::selectAndClickRow(ui->m_table_output, m_model, m_model.rowCount()-1);
    }
    });
    ui->m_table_output->setContextMenuPolicy(Qt::CustomContextMenu);
    ips::algo::contextMenu<ModelOutputConnections, OutputComment, QTableView, QToolButton>(&m_model, &m_comment, ui->m_table_output, ui->m_add, ui->m_rem, iMenu::tr("Add Connection"), iMenu::tr("Remove Connection"), iMenu::tr("Remove all Connections"));
}

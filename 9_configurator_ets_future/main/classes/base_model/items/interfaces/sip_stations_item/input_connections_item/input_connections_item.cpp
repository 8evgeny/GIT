#include "include/view.h"
#include "classes/algo.h"
#include "classes/app.h"
#include "classes/defines.h"
#include "ui_input_connections_item.h"
#include "dialogs/type_list/type_list.h"
#include "classes/base_model/items/station_item/station_item_settings/citgit_helper.h"
#include "classes/base_model/items/interfaces/sip_stations_item/input_connections_item/input_connections_instance.h"
#include "classes/base_model/items/interfaces/sip_stations_item/input_connections_item/input_connections_item.h"

InputConnectionsItem::InputConnectionsItem(BaseItem* item)
     : BaseItem(item)
{
}
InputConnectionsItem::~InputConnectionsItem()
{
    deshow();
}
void InputConnectionsItem::changeEvent(QEvent* event)
{
    if (event->type() == QEvent::LanguageChange) {
        if (ui)
            ui->retranslateUi(this);        
    }
    QWidget::changeEvent(event);
}
InputConnectionsItem* InputConnectionsItem::show(QWidget* content, QWidget* settings) {
    ips::algo::createContent(ui, content);
    ips::algo::removeChildren(settings);
    App::model()->settings()->setVisible(m_model.rowCount());
    m_settings.setupUi(settings);
    inputConnectionsItemInstance(ui, &m_model);
    mh::simple::view::selectAndClickRow(ui->m_table_input, m_model, m_model.rowCount()-1);
    instanceSignals();
    ui->m_rem->setEnabled(m_model.rowCount());
    return this;
}
void InputConnectionsItem::deshow()
{
    delete ui;
    ui = nullptr;
}
bool InputConnectionsItem::error(BaseItem*) const
{
    return true;
}
QString InputConnectionsItem::name(BaseItem*) const
{
    return iInputConnections::tr("Input connections");
}
QJsonValue InputConnectionsItem::serialize() const
{    
    return QJsonObject {
            {IPS_STR("comment_settings"), m_settings.serialize()}
            , {IPS_STR("connections"), m_model.serialize()}};
}
bool InputConnectionsItem::clear()
{    
    ips::algo::clear(&m_model);
    return true;
}
bool InputConnectionsItem::deserialize(QJsonObject const& data)
{
    IPS_JSON_EXTRACT_OBJECT(data, item, IPS_STR("InputConnections"))
    IPS_JSON_EXTRACT_OBJECT(item.toObject(), settings, IPS_STR("comment_settings"))
    IPS_JSON_EXTRACT_ARRAY(item.toObject(), connections, IPS_STR("connections"))
    m_settings.deserialize(settings.toObject());
    m_model.deserialize(connections.toArray());
    return true;
}
void InputConnectionsItem::instanceSignals() noexcept
{
    QObject::connect(&m_settings, &CommentSettings::textChanged, [this] (QString const& text) {
        if (text.isEmpty())
            return;

        m_model.setData(m_index, text, ModelInputConnections::CommentRole);
    });
    QObject::connect(ui->m_table_input, &QTableView::clicked, [this] (const QModelIndex& index) {
        if (!index.isValid())
            return;

        m_currentRow = index.row();
        m_index = index;
        m_row = index.row();
        auto connection = m_model.item(m_row);
        m_settings.setInputConnection(connection);
        m_settings.updateUi();
        mh::simple::view::selectRow(ui->m_table_input, index.row());
        if (index.column() == 1) {
            TypeListDialog<function_type::items_t> dlg(iFunction::tr("Functions"), function_type::defaultFunctionTypes(), connection.function());
            if (dlg.exec() == TypeListDialog<QStringList>::Accepted) {
                connection.setFunction(dlg.current());
                m_model.setItem(mh::selected(ui->m_table_input).row(), connection, {Qt::EditRole});
            }
        }
        if (index.column() == 2 && !cidGtList().isEmpty()) {
           TypeListDialog<QStringList> dlg(iFunction::tr("Destiations"), cidGtList(), connection.destination());
            if (dlg.exec() == TypeListDialog<QStringList>::Accepted) {
                 connection.setDestination(dlg.current());
                 m_model.setItem(mh::selected(ui->m_table_input).row(), connection, {Qt::EditRole});
            }
        }
    });
    QObject::connect(&m_model, &ModelInputConnections::dataChanged, [this] (const auto& index, const auto&, auto const& roles) {
        if (index.isValid() && roles.size() == 1 && roles[0] == Qt::EditRole) {
            m_settings.setInputConnection(m_model.item(index.row()));
        }
    });
    QObject::connect(&m_settings, &CommentSettings::functionChanged, this, [this] (QString const& function) {
        auto index =  mh::selected(ui->m_table_input);
        if (index.isValid()) {
            QModelIndex ind = m_model.index(index.row(), FUNCTION_COLUMN, index);            
            m_model.setData(ind, function);
        }
    });
    QObject::connect(&m_settings, &CommentSettings::inputNumberChanged, this, [this] (QString const& inputNumber) {
        auto index =  mh::selected(ui->m_table_input);
        if (index.isValid()) {
            QModelIndex ind = m_model.index(index.row(), INPUT_NUMBER_COLUMN, index);
            m_model.setData(ind, inputNumber);
        }
    });
    QObject::connect(&m_settings, &CommentSettings::functionChanged, this, [this] (QString const& function) {
        auto index = mh::selected(ui->m_table_input);
        if (index.isValid()) {
            QModelIndex ind = m_model.index(index.row(), FUNCTION_COLUMN, index);
            m_model.setData(ind, function);
        }
    });
    QObject::connect(&m_settings, &CommentSettings::destinationChanged, this, [this] (QString const& destination) {
        auto index =  mh::selected(ui->m_table_input);
        if (index.isValid()) {
            QModelIndex ind = m_model.index(index.row(), DESTINATION_COLUMN, index);
            m_model.setData(ind, destination);
        }
    });
    QObject::connect(&m_settings, &CommentSettings::priorityChanged, this, [this] (QString const& priority) {
        auto index =  mh::selected(ui->m_table_input);
        if (index.isValid()) {
            QModelIndex ind = m_model.index(index.row(), PRIORITY_COLUMN, index);
            m_model.setData(ind, priority);
        }
    });    
    QObject::connect(ui->m_add, &QPushButton::clicked, [this] (bool) {
        m_model.insertRow(m_model.rowCount());
        ui->m_rem->setEnabled(m_model.rowCount());
        App::model()->settings()->setVisible(true);
        mh::simple::view::selectAndClickRow(ui->m_table_input, m_model, m_model.rowCount()-1);
    });
    QObject::connect(ui->m_rem, &QToolButton::clicked, [this] () {
    ips::algo::selectedRowDelete<ModelInputConnections, QTableView>(&m_model, ui->m_table_input, m_currentRow);
    if (!m_model.rowCount()) {
        ui->m_rem->setDisabled(true);
        App::model()->settings()->setVisible(false);
        m_settings.clear();
        mh::simple::view::selectAndClickRow(ui->m_table_input, m_model, m_model.rowCount()-1);
    }
    });
    ui->m_table_input->setContextMenuPolicy(Qt::CustomContextMenu);
    ips::algo::contextMenu<ModelInputConnections, CommentSettings, QTableView, QToolButton>(&m_model, &m_settings, ui->m_table_input, ui->m_add, ui->m_rem, iMenu::tr("Add Connection"), iMenu::tr("Remove Connection"), iMenu::tr("Remove all Connections"));
}

#include "include/log.h"
#include "include/view.h"
#include "classes/algo.h"
#include "classes/defines.h"
#include "ui_groups_item.h"
#include "groups_instance.h"
#include "dialogs/limit_rows/limitrows.h"
#include "include/delegates/name_delegate.h"
#include "include/delegates/view_delegate.h"
#include "classes/base_model/items/groups/groups_item.h"
#include "classes/base_model/items/station_item/station_item.h"
#include "classes/base_model/items/global_parameters_item/cid_helper.h"

GroupsItem::GroupsItem(BaseItem* item)
    : BaseItem(item)
{
}
GroupsItem::~GroupsItem()
{
    delete ui;
}
void GroupsItem::changeEvent(QEvent* event)
{
    IPS_LANGUGE_EVENT(ui, this);
    QWidget::changeEvent(event);
}
void GroupsItem::updateListStationsAfterRemove(QString const& hash)
{
    for (auto i=0; i!=m_groupsSelector.modelGroups()->items().size(); ++i) {
        QModelIndex groupIndex = m_groupsSelector.modelGroups()->index(i, 0);
        LOG_ERROR(groupIndex.isValid(), "groupIndex must be is valid");
        QList<GroupsItem::station_t*> stations = m_groupsSelector.stations(groupIndex);
        if (!stations.isEmpty()) {
            auto size = stations.size();
            for (auto j=0; j!=size; ++j) {
                if (j < stations.size()) {
                    auto station = stations.value(j);
                    LOG_FATAL(station, "station can not be nullptr");
                    auto hahsStation = station->toHash();
                    if (hahsStation == hash) {
                        stations.removeAt(j);
                        --m_rowStation;
                        m_groupsSelector.modelGroups()->items()[m_rowGroup].setRow(m_rowStation);
                    }
                }
            }
            emit(changedStations(groupIndex, stations));
        }
    }    
    clearView();
}
template<typename T>
void GroupsItem::clearList(QList<T>& list) noexcept
{
    while (list.size())
            list.removeAt(0);

    list.clear();
}
void GroupsItem::hideAllStations(bool is) noexcept
{    
    auto items = App::model()->stationsItem()->model()->items();    
    for (auto i=0; i!=items.size(); ++i)
         ui->m_table_stations->setRowHidden(i, is);
}
void GroupsItem::stationsViewer(const QModelIndex& currentGroup) noexcept
{
    clearView();
    LOG_ERROR(currentGroup.isValid(), "currentGroup must be is valid");
    auto const& stations = m_groupsSelector.stations(currentGroup);
    LOG_ERROR(!stations.isEmpty(), "stations is not empty");
    for (auto i=0; i!=stations.size(); ++i)
         m_modelView.insertItem(i, stations.value(i));
}
void GroupsItem::clearView() noexcept
{
    ips::algo::clear<ModelTableView>(&m_modelView);
}
void GroupsItem::clearHidden() noexcept
{
    while (m_hidden.size())
            m_hidden.removeAt(0);

    m_hidden.clear();
}
void GroupsItem::viewStations() noexcept
{   
      for (auto i=0; i!=m_hiddenStations.size(); ++i) {
          auto is = m_hiddenStations.value(i);
          ui->m_table_stations->setRowHidden(i, is);
      }
}
void GroupsItem::setHiddenStations() noexcept
{
    if (m_currentGroup.isValid()) {
        clearList<bool>(m_hiddenStations);
        auto items = App::model()->stationsItem()->model()->items();
        for (auto i=0; i <= items.size()-1; ++i) {
            m_hiddenStations.insert(i, false);
            if (ui->m_table_stations->isRowHidden(i))
                m_hiddenStations[i] = true;
        }
    }
}
QList<bool> GroupsItem::hiddenStations() const noexcept
{
    if (m_currentGroup.isValid()) {
        QList<bool> hidden;
        auto items = App::model()->stationsItem()->model()->items();
        for (auto i=0; i!=items.size(); ++i) {
            hidden.insert(i, false);
            if (ui->m_table_stations->isRowHidden(i))
                hidden[i] = true;
        }
        return hidden;
    }
    return {};
}
bool GroupsItem::containsHash(QString const& hash, QModelIndex const& group) noexcept
{
    QList<QString> hashList;
    QList<GroupsItem::station_t*> stations = m_groupsSelector.stations(group);
    for (auto station : stations)
         hashList.append(station->toHash());
    return hashList.contains(hash);
}
int GroupsItem::findFreeStation() noexcept
{
    auto items = App::model()->stationsItem()->model()->items();
    for (auto i=0; i!=items.size(); ++i) {
        if (!ui->m_table_stations->isRowHidden(i))
            return i;
    }
    return -1;
}
void GroupsItem::createGroup(bool)
{
    ui->m_delete->setEnabled(true);
    m_groupsSelector.modelGroups()->items().value(m_rowGroup).setRow(0);
    m_groupsSelector.modelGroups()->insertRows(m_groupsSelector.modelGroups()->rowCount({}), 1);
    mh::simple::view::selectRow(ui->m_table_groups, m_groupsSelector.modelGroups()->items().size()-1);
    ui->m_left->setVisible(App::model()->stationsItem()->model()->rowCount() && m_groupsSelector.modelGroups()->rowCount() && m_modelView.items().size());
    ui->m_right->setVisible(App::model()->stationsItem()->model()->rowCount() && m_groupsSelector.modelGroups()->rowCount() && m_modelView.items().size());
    emit(updateGroupsList(m_groupsSelector.modelGroups()->items()));
}
void GroupsItem::deleteGroup(bool)
{
    clearView();
    mh::simple::view::selectRow(ui->m_table_stations, 0);
    auto const& indx = mh::selected(ui->m_table_groups);
    if (indx.isValid()) {
       m_groupsSelector.modelGroups()->removeRows(indx.row(), 1);
       emit(updateGroupsList(m_groupsSelector.modelGroups()->items()));
       m_currentGroup = m_groupsSelector.modelGroups()->index(m_groupsSelector.modelGroups()->rowCount(), 0);
       mh::simple::view::selectRow(ui->m_table_groups, (indx.row() == 0) ? 0 : indx.row() - 1);
       ui->m_delete->setEnabled(m_groupsSelector.modelGroups()->rowCount());
       ui->m_right->setVisible(App::model()->stationsItem()->model()->rowCount() && m_groupsSelector.modelGroups()->rowCount() && m_modelView.items().size());
       ui->m_left->setVisible(App::model()->stationsItem()->model()->rowCount() && m_groupsSelector.modelGroups()->rowCount());
       if (!m_groupsSelector.modelGroups()->rowCount())
           hideAllStations(true);
       if (m_groupsSelector.modelGroups()->rowCount()) {
           auto deleteGroup = mh::selected(ui->m_table_groups);
           ui->m_table_groups->clicked(deleteGroup);
       }
    }
}
void GroupsItem::selectRows() noexcept
{
    if (!m_modelView.items().isEmpty() && m_rowGroup >= 0 ) {
        mh::simple::view::selectRow(ui->m_table_groups, m_rowGroup);
        mh::simple::view::selectRow(ui->m_table_view, m_modelView.items().size()-1);        
    }
}
void GroupsItem::currentStationInSelectGroup() noexcept
{
    if (!m_hidden.isEmpty()) {
        QList<GroupsItem::station_t*> stations = m_groupsSelector.stations(m_currentGroup);
        m_rowStation = (stations.size()) ? stations.size() : 0;
    }
    else
        m_rowStation = m_groupsSelector.modelGroups()->items().value(m_rowGroup).currentRow();
}
void GroupsItem::selectGroup(const QModelIndex& index)
{    
    LOG_ERROR(index.isValid(), "index group must be is valid!");
    clearView();
    m_currentGroup = index;
    m_rowGroup = index.row();    
    hideAllStations(false);
    viewVisibleStations(index);

    if (!m_groupsSelector.stations(m_currentGroup).isEmpty())
        stationsViewer(m_currentGroup);
    selectRows();
    currentStationInSelectGroup();
    ui->m_left->setVisible(!checkingSizeView());
    ui->m_right->setVisible(App::model()->stationsItem()->model()->rowCount() && m_groupsSelector.modelGroups()->rowCount() && m_modelView.items().size());
    clearHidden();
}
int GroupsItem::positionStationHidden(QString const& hash) noexcept
{
    auto items = App::model()->stationsItem()->model()->items();
    for (auto i=0; i!=items.size(); ++i) {
        auto item = items.value(i);
        if (item->toHash() == hash)
            return i;
    }
    return -1;
}
bool GroupsItem::checkingSizeView() noexcept
{
    if ((m_modelView.items().size() >= App::model()->stationsItem()->model()->rowCount()) || findFreeStation() == -1)
        return true;
    return false;
}
void GroupsItem::addStation(GroupsItem::station_t* station, int pos) noexcept
{
    QList<GroupsItem::station_t*> stations = m_groupsSelector.stations(m_currentGroup);
    stations.append(station);
    m_modelView.insertItem(pos, station);
    emit(changedStations(m_currentGroup, stations));
    ui->m_table_stations->setRowHidden(positionStationHidden(station->toHash()), true);
    setHiddenStations();
    mh::simple::view::selectRow(ui->m_table_view, m_modelView.items().size()-1);
    ++m_rowStation;
    m_groupsSelector.modelGroups()->items()[m_rowGroup].setRow(m_rowStation);
    ui->m_right->setVisible(App::model()->stationsItem()->model()->rowCount() && m_groupsSelector.modelGroups()->rowCount() && m_modelView.items().size());
    if (checkingSizeView())
        ui->m_left->setVisible(false);
}
void GroupsItem::addStationToPosition(int stNumber, int pos) noexcept
{
    LOG_ERROR((stNumber >= 0 && stNumber < App::model()->stationsItem()->model()->rowCount()) ? true : false, "stNumber out of range!");
    auto station = App::model()->stationsItem()->model()->items().value(stNumber);
    LOG_FATAL(station, "station can not be null");
    if (!ui->m_table_stations->isRowHidden(stNumber))
        addStation(station, pos);
    else {
         auto stationFree = App::model()->stationsItem()->model()->items().value(findFreeStation());
         LOG_FATAL(stationFree, "station can not be null");
         addStation(stationFree, pos);
    }
}
void GroupsItem::addStation(bool)
{
    if (m_rowStation <= App::model()->stationsItem()->model()->rowCount()-1)
       addStationToPosition(m_rowStation, m_rowStation);
}
void GroupsItem::removeStation(int pos) noexcept
{
    ui->m_left->setVisible(true);
    m_modelView.removeRow(pos);
    ui->m_right->setVisible(App::model()->stationsItem()->model()->rowCount() && m_groupsSelector.modelGroups()->rowCount() && m_modelView.items().size());
    mh::simple::view::selectRow(ui->m_table_view, m_modelView.items().size()-1);
    QList<GroupsItem::station_t*> stations = m_groupsSelector.stations(m_currentGroup);
    GroupsItem::station_t* station = stations.value(pos);
    ui->m_table_stations->setRowHidden(positionStationHidden(station->toHash()), false);
    setHiddenStations();
    stations.removeAt(pos);
    emit(changedStations(m_currentGroup, stations));
}
void GroupsItem::removeSelectStation(const QModelIndex& index)
{
    LOG_ERROR(index.isValid(), "index must be is Valid");
    if (!m_modelView.items().isEmpty() && m_rowStation >= 0) {        
        removeStation(index.row());
        if (m_rowStation > 0)
            --m_rowStation;
        m_groupsSelector.modelGroups()->items()[m_rowGroup].setRow(m_rowStation);
    }
}
void GroupsItem::removeStation(bool)
{
    if (!m_modelView.items().isEmpty() && m_rowStation > 0) {
        removeStation(m_modelView.items().size()-1);        
        --m_rowStation;
        m_groupsSelector.modelGroups()->items()[m_rowGroup].setRow(m_rowStation);
    }
}
void GroupsItem::initConnections() noexcept
{
    connect(ui->m_create, SIGNAL(clicked(bool)), this, SLOT(createGroup(bool)));
    connect(ui->m_delete, SIGNAL(clicked(bool)), this, SLOT(deleteGroup(bool)));
    connect(ui->m_right, SIGNAL(clicked(bool)), this, SLOT(removeStation(bool)));
    connect(ui->m_left, SIGNAL(clicked(bool)), this, SLOT(addStation(bool)));
    connect(ui->m_table_groups, SIGNAL(clicked(const QModelIndex&)), this, SLOT(selectGroup(const QModelIndex&)));
    connect(ui->m_table_stations, SIGNAL(clicked(const QModelIndex&)), this, SLOT(selectStation(const QModelIndex&)));
    connect(ui->m_table_view, SIGNAL(clicked(const QModelIndex&)), this, SLOT(removeSelectStation(const QModelIndex&)));
    connect(this, SIGNAL(changedStations(const QModelIndex&, QList<StationItem*>)), &m_groupsSelector, SLOT(update(const QModelIndex&, QList<StationItem*>)));
}
void GroupsItem::selectStation(const QModelIndex& index)
{
    LOG_ERROR(index.isValid(), "index must be valid!");
    if (m_rowStation <= App::model()->stationsItem()->model()->rowCount()-1)
        addStationToPosition(index.row(), m_rowStation);
}
bool GroupsItem::groupsIsEmptyChecker() noexcept
{
    for (auto i=0; i!=m_groupsSelector.modelGroups()->items().size(); ++i) {
        QModelIndex indexGroup = m_groupsSelector.modelGroups()->index(i, 0);
        LOG_ERROR(indexGroup.isValid(), "indexGroup must be is Valid");
        if (!m_groupsSelector.stations(indexGroup).isEmpty())
            return true;
    }
    return false;
}
void GroupsItem::viewVisibleStations(QModelIndex currentGroup) noexcept
{
    auto group = m_groupsSelector.modelGroups()->items().value(currentGroup.row());
    auto stationsOfGroup = group.stationList();
    for (auto j=0; j < stationsOfGroup.size(); ++j) {
         auto stationOfGroup = stationsOfGroup.value(j);
         for (auto i=0; i < App::model()->stationsItem()->model()->items().size(); ++i) {
              auto station = App::model()->stationsItem()->model()->items().value(i);
              if (stationOfGroup->toHash() == station->toHash())
                  ui->m_table_stations->setRowHidden(positionStationHidden(station->toHash()), true);
       }
    }
}
GroupsItem* GroupsItem::show(QWidget* content, QWidget* settings)
{        
    ips::algo::removeChildren(content);
    ips::algo::removeChildren(settings);
    ips::algo::createContent(ui, content);
    GroupsItemInstance(ui, m_groupsSelector.modelGroups()->rowCount(), m_modelView.items().size());
    connect(App::model()->stationsItem()->model(), SIGNAL(removeStation(QString const&)), this, SLOT(updateListStationsAfterRemove(QString const&)));

    if (App::model()->stationsItem()->model()->items().isEmpty())
        clearList<GroupCall>(m_groupsSelector.modelGroups()->items());

    if (!groupsIsEmptyChecker())
        hideAllStations(true);
    else
        viewStations();

    if (m_groupsSelector.modelGroups()->rowCount() ) {
            auto groupRow = m_groupsSelector.modelGroups()->rowCount()-1;
            QModelIndex indexGroup = m_groupsSelector.modelGroups()->index(groupRow, 0, {});
            LOG_ERROR(indexGroup.isValid(), "indexGroup must be is valid!");
            viewVisibleStations(indexGroup);
        }

    m_rowGroup = 0;
    if (App::model()->stationsItem()->model()->items().size())
        initConnections();

    m_countStations = App::model()->stationsItem()->model()->items().size();
    return this;
}
void GroupsItem::deshow()
{
    delete ui;
    ui = nullptr;
}
bool GroupsItem::error(BaseItem*) const
{
    return true;
}
QString GroupsItem::name(BaseItem*) const
{
    return GroupsItem::tr("Groups");
}
QJsonArray GroupsItem::serializeHiddenStations() const noexcept
{
    QJsonArray res;
    auto tree = App::model()->tree();
    auto treeIndex = mh::selected(tree);
    LOG_ERROR(treeIndex.isValid(), "treeIndex must be is Valid");
    if (!App::model()->groupItem()->groupsIsEmptyChecker() && treeIndex.row() == 3) {
        QList<bool> hideStations = hiddenStations();
        for (auto is : hideStations)
             res.push_back(QJsonObject {{IPS_STR("position"), is}});
    }
    return res;
}
QList<bool> GroupsItem::deserializeHiddenStations(QJsonValue value) const noexcept
{
    QList<bool> posStation;
    auto posList = value.toArray();
    for (auto is : posList)
        posStation.append(is.toObject().value(IPS_STR("position")).toBool());
    return posStation;
}
QJsonValue GroupsItem::serialize() const
{
    QJsonObject obj;
    return QJsonObject {{IPS_STR("groups_calls"), m_groupsSelector.serialize()}};
}
bool GroupsItem::clear()
{
    clearView();
    m_groupsSelector.clear();
    return false;
}
bool GroupsItem::deserialize(QJsonObject const& data)
{
     auto const& groups = data.value(IPS_STR("Groups"));
     LOG_ERROR(!groups.isUndefined() || !groups.isObject(), "Groups is not an object or invalid");

     auto const &group = groups.toObject();
     LOG_ERROR(!group.isEmpty(), "Group is Empty");

     auto const& groupcalls = group.value(IPS_STR("groups_calls"));     
     if (groupcalls.isUndefined() || !groupcalls.isArray())
        return LOG_ERROR(false, "groupcalls is not found or invalid");
     else
          m_groupsSelector.deserialize(groupcalls.toArray());
     return true;
}

#pragma once
#include <QObject>
#include "classes/base_model/items/base_item.h"
#include "classes/base_model/items/stations_item/stations_item.h"
#include "classes/base_model/items/station_item/station_item.h"
#include "classes/base_model/items/groups/models/calls/group_calls/group_calls.h"
#include "classes/base_model/items/groups/models/calls/groups_calls.h"
#include "classes/base_model/items/groups/models/digital_stations/view.h"
#include "classes/base_model/items/groups/groups_selector/groups_selector.h"
#include "classes/base_model/items/groups/models/digital_stations/digital_stations.h"

#define POS_CID_IN_MODEL_STATIONS       0
#define POS_NAME_IN_MODEL_VIEW          0
#define POS_NAME_IN_MODEL_GROUPS        0
#define POS_GID_IN_MODEL_GROUPS         1
#define POS_NAME_IN_MODEL_STATIONS      1
#define POS_CID_IN_MODEL_VIEW           1
#define TABLE_STATIONS_DEFAULT_POSITION 0

IPS_UI(GroupsItem)

class GroupsItem final: public QWidget, public BaseItem {
private:
    Q_OBJECT
    IPS_DISABLE_COPY(GroupsItem)
    friend class TestGroupsItem;

public:
     using class_name       = GroupsItem;
     using ui_t             = Ui::GroupsItem;
     using station_t        = StationItem;
     using itemsGroups_t    = QList<GroupCall>;

public:
     signals:     
     void updateGroupsList(itemsGroups_t const& groups);
     void changedStations(const QModelIndex& index, QList<StationItem*> stations);

public slots:
     void selectStation(const QModelIndex& index);
     void createGroup(bool checked);
     void deleteGroup(bool checked);
     void selectGroup(const QModelIndex& index);
     void addStation(bool);
     void removeStation(bool);
     void removeSelectStation(const QModelIndex& index);
     void updateListStationsAfterRemove(QString const& hash);

private:
     template<typename T>
     void clearList(QList<T>& list) noexcept;

private:
     void clearView() noexcept;
     void clearHidden() noexcept;
     int  findFreeStation() noexcept;
     void selectRows() noexcept;
     void setSettings() noexcept;
     bool checkingSizeView() noexcept;     
     void hideAllStations(bool is) noexcept;
     void setHiddenStations() noexcept;
     void currentStationInSelectGroup() noexcept;
     void viewVisibleStations(QModelIndex currentGroup = {}) noexcept;
     int  positionStationHidden(QString const& hash) noexcept;
     bool containsHash(QString const& hash, QModelIndex const& group) noexcept;
     void addStation(GroupsItem::station_t* station, int pos) noexcept;
     void addStationToPosition(int stNumber, int pos) noexcept;
     void initConnections() noexcept;
     void removeStation(int pos) noexcept;
     void stationsViewer(const QModelIndex& currentGroup) noexcept;
     void changeEvent(QEvent* event) override final;
     void addStationToGroup(int row) noexcept;     
     QJsonArray serializeHiddenStations() const noexcept;
     QList<bool> deserializeHiddenStations(QJsonValue value) const noexcept;

public:
     explicit GroupsItem(BaseItem* item = nullptr);
     virtual ~GroupsItem() final;

public:
     virtual GroupsItem* show(QWidget* content, QWidget* settings = {}) final;
     virtual void deshow() final;
     virtual bool error(BaseItem* Item = nullptr) const final;
     virtual QString name(BaseItem* Item = nullptr) const final;
     virtual QJsonValue serialize() const final;
     virtual bool deserialize(QJsonObject const& obj) final override;
     virtual bool clear() final;

public:
     bool groupsIsEmptyChecker() noexcept;
     QList<bool> hiddenStations() const noexcept;
     void viewStations() noexcept;
     GroupsSelector* group() noexcept {
         return &m_groupsSelector;
     }
     ModelTableView* view() noexcept {
         return &m_modelView;
     }

private:
     ui_t* ui = nullptr;
     int m_rowGroup = 0;
     int m_rowStation = 0;
     QList<bool> m_hidden;
     int m_countStations = 0;
     QList<bool> m_hiddenStations;
     ModelTableView m_modelView;
     QModelIndex m_currentGroup = {};
     GroupsSelector m_groupsSelector;
};

#pragma once
#include "classes/app.h"
#include "include/delegates/name_delegate.h"
#include "include/delegates/view_delegate.h"
#include "classes/base_model/items/groups/groups_item.h"
#include "classes/base_model/items/stations_item/stations_item.h"
#include "classes/base_model/items/groups/models/calls/groups_calls.h"

#define STRING_MAX_LENGTH_NAMEGROUP     64
#define MAX_LENGTH_CID                  4
#define TYPE_POSITION                   2
#define IP_POSITION_STATIONS            3

template <class U>
class GroupsInstance final {
  public:
    using class_name = GroupsInstance;
    using ui_t       = U;

   private:
    ui_t* ui = nullptr;
    int deleteCount = {};
    int size = {};

   private:
    void instanceWidgets() noexcept {
         setButtonSettings();
         setModelsSettings();
         setGroupsSettings();
         setViewSettings();
         setStationsSettings();
    }
    void setButtonSettings() noexcept
    {
        ui->m_delete->setEnabled(deleteCount);
        ui->m_create->setEnabled(App::model()->stationsItem()->model()->rowCount());
        ui->m_left->setVisible(!ui->m_table_stations->isHidden() && App::model()->stationsItem()->model()->rowCount() && deleteCount && size);
        ui->m_right->setVisible(App::model()->stationsItem()->model()->rowCount() && deleteCount && size);
    }
    void setGroupsSettings() noexcept
    {
        ui->m_table_groups->setSelectionMode(QAbstractItemView::SingleSelection);
        ui->m_table_groups->verticalHeader()->hide();
        ui->m_table_groups->setShowGrid(false);
        ui->m_table_groups->horizontalHeader()->setHighlightSections(false);
        ui->m_table_groups->horizontalHeader()->setDefaultSectionSize(0);
        ui->m_table_groups->setSelectionBehavior(QAbstractItemView::SelectRows);
        ui->m_table_groups->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        ui->m_table_groups->setItemDelegateForColumn(0, new NameDelegate(STRING_MAX_LENGTH_NAMEGROUP));
        ui->m_table_groups->setItemDelegateForColumn(1, new ViewDelegate(MAX_LENGTH_CID));
        ui->m_table_groups->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
        ui->m_table_groups->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);

    }
    void setViewSettings() noexcept
    {
        ui->m_table_view->setSelectionMode(QAbstractItemView::SingleSelection);
        ui->m_table_view->setSelectionBehavior(QAbstractItemView::SelectRows);
        ui->m_table_view->setEditTriggers(QAbstractItemView::NoEditTriggers);
        ui->m_table_view->setColumnHidden(TYPE_POSITION, true);
        ui->m_table_view->setColumnHidden(IP_POSITION_STATIONS, true);
        ui->m_table_view->verticalHeader()->hide();
        ui->m_table_view->setShowGrid(false);
        ui->m_table_view->horizontalHeader()->setHighlightSections(false);
        ui->m_table_view->horizontalHeader()->setDefaultSectionSize(0);
        ui->m_table_view->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
        ui->m_table_view->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    }
    void setStationsSettings() noexcept
    {
        ui->m_table_stations->setSelectionMode(QAbstractItemView::SingleSelection);
        ui->m_table_stations->setSelectionBehavior(QAbstractItemView::SelectRows);
        ui->m_table_stations->setEditTriggers(QAbstractItemView::NoEditTriggers);
        ui->m_table_stations->setColumnHidden(TYPE_POSITION, true);
        ui->m_table_stations->setColumnHidden(IP_POSITION_STATIONS, true);
        ui->m_table_stations->verticalHeader()->hide();
        ui->m_table_stations->setShowGrid(false);
        ui->m_table_stations->horizontalHeader()->setHighlightSections(false);
        ui->m_table_stations->horizontalHeader()->setDefaultSectionSize(0);
        ui->m_table_stations->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        ui->m_table_stations->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
        ui->m_table_stations->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    }
    void setModelsSettings() noexcept
    {
        ui->m_table_groups->setModel(App::model()->groupItem()->group()->modelGroups());
        ui->m_table_view->setModel(App::model()->groupItem()->view());
        ui->m_table_stations->setModel(App::model()->stationsItem()->model());
    }

   public:
    explicit GroupsInstance(ui_t* ui, int deleteCount, int size)
        : ui(ui)
        , deleteCount(deleteCount)
        , size(size)
    {
    }
    void operator()() {
        instanceWidgets();
    }
};

template <class U>
inline void GroupsItemInstance(U* ui, int deleteCount, int size) noexcept {
    GroupsInstance<U> instance(ui, deleteCount, size);
    instance();
}

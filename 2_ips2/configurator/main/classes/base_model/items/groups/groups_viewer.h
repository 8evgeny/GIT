#pragma once
#include <QObject>
#include "classes/app.h"
#include "classes/base_model/items/groups/groups_item.h"
#include "classes/base_model/items/groups/models/calls/groups_calls.h"
#include "classes/base_model/items/groups/models/digital_stations/view.h"
#include "classes/base_model/items/groups/groups_selector/groups_selector.h"

class GroupsViewer final : public QObject
{    
    Q_OBJECT

public:
     using class_name = GroupsViewer;
     using ui_t       = Ui::GroupsItem;

public:
    void settings() noexcept;

public:
    explicit GroupsViewer(Ui::GroupsItem* ui = nullptr);

private:
    ui_t* ui = nullptr;
};

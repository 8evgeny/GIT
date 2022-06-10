#pragma once
#include <QWidget>
#include "classes/base_model/items/base_item.h"
#include "include/typedefs.h"
#include "classes/base_model/items/interfaces/sip_stations_item/sip_stations_item.h"

IPS_UI(InterfacesItem)

class InterfacesItem final : public QWidget, public BaseItem {
    Q_OBJECT
    IPS_DISABLE_COPY(InterfacesItem)

public:
     using class_name = InterfacesItem;
     using ui_t       = Ui::InterfacesItem;
     using inherited  = BaseItem;

public:
     explicit InterfacesItem(BaseItem* item = nullptr);
     virtual ~InterfacesItem() override;

public:
     virtual InterfacesItem* show(QWidget* content, QWidget* settings = {}) override;
     virtual void deshow() override;
     virtual bool error(BaseItem* Item = nullptr) const override;
     virtual QString name(BaseItem* Item = nullptr) const override;
     virtual QJsonValue serialize() const override;
     virtual bool deserialize(QJsonObject const& obj) final override;
     virtual bool clear() final;

private:
    ui_t* ui = nullptr;
};

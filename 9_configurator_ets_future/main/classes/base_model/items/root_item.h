#pragma once
#include "base_item.h"

class RootItem final: public BaseItem {
private:
    using inherited = BaseItem;

public:
    using BaseItem::BaseItem;

public:
    virtual RootItem* show(QWidget*, QWidget*) final {
        return this;
    }
    virtual void deshow() final {
    }
    virtual bool error(BaseItem* = nullptr) const final {
        return true;
    }
    virtual QString name(BaseItem* = nullptr) const final {
        return IPS_STR("RootItem");
    }
    virtual QJsonValue serialize() const final {
        return {};
    }
    virtual bool deserialize(QJsonObject const&) final {
        return true;
    }   
    virtual bool clear()  final {
        return true;
    }
};

#pragma once
#include "main/classes/base_model/items/base_item.h"
#include "include/validators/string_validator.h"
#include "main/classes/cid.h"
#include "classes/cids_container.h"
#include "classes/algo.h"
#include <QList>

#define MAX_CID_VALUE 9999

IPS_UI(GlobalParameters)

class GlobalParametersItem final: public QWidget, public BaseItem {
    Q_OBJECT

    friend class StationItem;
    friend class TestGlobalParametersItem;

private:
    using inherited = BaseItem;
    using ui_t = Ui::GlobalParameters;
    using isValid = QValidator::State;

private:
    void instanceDataGlobal();
    void instanceSignalsGlobal();
    void editingCID(QLineEdit* cidType, uint& value) noexcept;
    void editingIP(QLineEdit* ipType, QString& value) noexcept;

public:
    using BaseItem::BaseItem;
    virtual ~GlobalParametersItem() override;

public:    
    virtual GlobalParametersItem* show(QWidget* content, QWidget* settinsg = {}) override;
    virtual void deshow() final;
    virtual bool error(BaseItem* = nullptr) const  final {
        return true;
    }
    virtual QString name(BaseItem* Item = nullptr) const override;
    virtual QJsonValue serialize() const override;
    virtual bool deserialize(QJsonObject const& data) override;
    virtual bool clear()  final;

public slots:
    void dataChanged();

public:
    uint publicAddress() const {
        return m_pa;
    }
    uint duplex() const {
        return m_duplex;
    }
    uint alarm() const {
        return m_alarm;
    }
    uint conference() const {
        return m_conference;
    }
    uint groupPa() const {
        return m_group_pa;
    }
    uint dynamicGroupCall() const {
        return m_dynamic_group_call;
    }
    cid_t callerid() const {
        return m_callerid;
    }
    QString const& ip() const {
        return m_ip;
    }
    QString const& netmask() const {
        return m_netmask;
    }
    QString const& gateway() const {
        return m_gateway;
    }

public:
    QList<CidsContainer*>& cids() noexcept;
    void changeEvent(QEvent *event) override final;

private:
    ui_t* ui = nullptr;
    uint m_pa = 60U;
    uint m_duplex = 50U;
    uint m_group_pa = 65U;
    uint m_conference = 40U;
    uint m_dynamic_group_call = 55U;
    uint m_alarm = 70U;
    cid_t m_callerid = 1000U;
    QString m_ip = IPS_STR("192.168.10.1");
    QString m_netmask = IPS_STR("255.255.255.0");
    QString m_gateway;
    QList<CidsContainer*> m_cids = {};
};

#pragma once
#include "model_rules/model_rules.h"
#include "model_destination/model_destination.h"
#include "main/messages/dialogs/messages/messages_dialog.h"
#include "main/classes/base_model/items/base_item.h"
#include "sip_settings/sip_settings.h"

IPS_UI(SipStationsItem)

class OutputConnectionsItem;
class InputConnectionsItem;

class SipStationsItem final : public QWidget, public BaseItem {
    Q_OBJECT
    IPS_DISABLE_COPY(SipStationsItem)

public:
     using class_name = SipStationsItem;
     using ui_t       = Ui::SipStationsItem;
     using inherited  = BaseItem;
     using dest_t     = ModelDestination;

public:
     signals:
     void dataChanged();

private:  
     void changeEvent(QEvent* event) override;
     void installSignals() noexcept;
     template <class T1, class T2>
     void selectedRowDelete(T1* model, T2* view) noexcept;
     inline auto destsList() noexcept;

public:
     explicit SipStationsItem(BaseItem* item = nullptr);
     virtual ~SipStationsItem() override;

public:
     virtual SipStationsItem* show(QWidget* content, QWidget* settings = {}) override;
     virtual void deshow() override;
     virtual bool error(BaseItem* Item = nullptr) const override;
     virtual QString name(BaseItem* Item = nullptr) const override;
     virtual QJsonValue serialize() const override;
     virtual bool deserialize(QJsonObject const& obj) final override;
     virtual bool clear() final;

public:
     dest_t* model() noexcept {
         return m_modelDest;
     }

private:     
     ui_t* ui = nullptr;
     ModelDestination* m_modelDest;
     ModelRules* m_modelRules;
     QModelIndex m_index = {};
     int m_currentRow = 0;
     QString m_prefix = {};
     uint m_cid = 0;
     SipSettings* m_settings = nullptr;
};

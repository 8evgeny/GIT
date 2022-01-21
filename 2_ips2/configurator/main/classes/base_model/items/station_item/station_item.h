#pragma once
#include "model_keys/model_keys.h"
#include "classes/base_model/items/station_item/station_item_settings/station_item_settings.h"
#include "classes/base_model/items/groups/models/calls/group_calls/group_calls.h"

#define MAX_COUNT_KEYS 48

IPS_UI(StationItem)

class GroupCall;
class StationItemSettings;

class StationItem final: public QWidget, public BaseItem, public Station {
     Q_OBJECT
     IPS_DISABLE_COPY(StationItem)

     friend class TestStationItem;
     friend class TestStationItems;

public:
     using station_t   = Station;
     using inherited   = BaseItem;
     using class_name  = StationItem;
     using keys_t      = QList<Key>;
     using ui_t        = Ui::StationItem;
     using itemsGroups_t = QList<GroupCall>;

private slots:
     void keysSelectionChanged(const QItemSelection&, const QItemSelection&);

private:
     void instanceConnectionToSettings();
     void initDataFromTypeTable();     
     void instanceSignals();         
     void changeEvent(QEvent *event) override;
     void setCurrentValuesStringToUI(const QModelIndex& index);

public:
     virtual StationItem* show(QWidget* content, QWidget* set = {}) override;
     virtual void deshow() override;
     virtual bool error(BaseItem* Item = nullptr) const override;
     virtual QString name(BaseItem* Item = nullptr) const override;
     virtual QJsonValue serialize() const final;
     virtual bool deserialize(QJsonObject const& data) final override;
     virtual bool clear() final;

public:
     void selectedRowDelete() noexcept;

public:
     explicit StationItem(BaseItem* item = nullptr);
     virtual ~StationItem() final;
     static class_name* defValue(BaseItem* parent, StationType type, uint cid, uint id, QString const& ip, QString const& mask, QString const& gateway);
     static class_name* defValue(BaseItem* parent, uint cid, uint id, QString const& ip, QString const& mask, QString const& gateway);
     ModelKeys const& model() {
         return m_model;
     }     
     void setApp(App* app) noexcept {
         m_app = app;
     }
     bool isChecked() const noexcept {
         return m_isChecked;
     }
     void setChecked(bool is) noexcept {
         m_isChecked = is;
     }
     void setHash(QString const& hash) noexcept {
         m_hash = hash;
     }
     QString const& toHash() noexcept {
         return m_hash;
     }
     void setModelTones(TracksModel* modelTones) noexcept {
         m_modelTones = modelTones;
     }

private slots:
     void updaterGroupsList(itemsGroups_t const&);

private:
     QString m_hash;
     bool m_isChecked;
     ui_t* ui = nullptr;
     int m_row = -1;          
     App* m_app = nullptr;
     ModelKeys m_model;     
     QList<bool> m_defaultValues = {};     
     QStringList m_groupsList = {};
     StationItemSettings m_settings;
     TracksModel* m_modelTones = nullptr;
};

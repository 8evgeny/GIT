#pragma once
#include "main/classes/base_model/items/station_item/model_keys/key.h"
#include "main/classes/base_model/items/stations_item/model_stations/station.h"
#include "classes/base_model/items/station_item/active_lines_serializator/active_lines_serializator.h"
#include "main/messages/model_tracks/tracks_model.h"
#include "editor_button.h"

#define MAX_CID_VALUE 9999

IPS_UI(StationItemSettings)

class StationItemSettings final: public QObject {
     Q_OBJECT
     IPS_DISABLE_COPY(StationItemSettings)

 public:
     using class_name  = StationItemSettings;
     using ui_t        = Ui::StationItemSettings;

 private:
     void editingIP(QLineEdit* ipType, QString& value) noexcept;
     void setStation(Station* station) noexcept;
     void initPretones() noexcept;
     void hidePretone() noexcept;     

 public:
     StationItemSettings();
     ~StationItemSettings();
     void hideSettings(bool is) noexcept;     
     void setupUi(QWidget* parent, Station* station, StationItem* StationItem);
     void setKeySettingsEnabled(bool is, int countKeys = 0) noexcept;
     void setKey(Key const& key) noexcept;
     void instanceStationSignals() noexcept;
     void instanceKeySignals() noexcept;
     void sendKey(uint priority, QString const& destination = {}, bool pretoneChanged = true) noexcept;
     void retranslateUi();
     void setSettingsPretone() noexcept;

 private slots:
     void pretoneChanged(const QString& pretone) noexcept;
     void pretonePlayChanged(QString const&) noexcept;
     void dataStationChanged();
     void dataKeyChanged(QString const& destination = {});
     void dataKeyChangedFlag(bool);
     void functionChanged();

 signals:
     void textChanged(QString const& text);
     void changedCid(cid_t cid);
     void keyChanged(Key& key, bool checkedPritone = false);
     void stationChanged(Station const& key);     

 public:
     void setCidStatus(bool status) noexcept {
         m_cidStatus = status;
     }
     void setModelTones(TracksModel* model) noexcept {
         m_model = model;
     }

 private:
     QString m_duration = {};
     QString m_pretone = {};
     bool m_cidStatus = false;
     bool m_func_call = false;     
     ui_t* ui = nullptr;
     EditorButton* m_edit = nullptr;
     QHBoxLayout* m_lay = nullptr;
     StationItem* m_station = nullptr;
     QWidget* m_parent = nullptr;
     QString m_ip;
     QString m_mask;
     QString m_gateway;
     cid_t m_cid;
     uint m_id;
     DurationType m_savePretone;
     TracksModel* m_model = nullptr;
};

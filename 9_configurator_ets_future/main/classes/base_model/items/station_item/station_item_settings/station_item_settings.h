/*!
 * @file station_item_settings.h
 * @file station_item_settings.cpp
 * @brief This file contains a class for settings station and current key
*/
#pragma once
#include <QMessageBox>
#include "main/classes/base_model/items/station_item/model_keys/key.h"
#include "main/classes/base_model/items/stations_item/model_stations/station.h"
#include "classes/base_model/items/station_item/active_lines_serializator/active_lines_serializator.h"
#include "main/messages/model_tracks/tracks_model.h"
#include "editor_button.h"

#define MAX_CID_VALUE 9999

IPS_UI(StationItemSettings)
/*! 
\defgroup station_item_settings_module Class for configuring station and current key parameters
*/
class StationItemSettings final: public QObject {
     Q_OBJECT
     IPS_DISABLE_COPY(StationItemSettings)

 public:
     using class_name  = StationItemSettings;
     using ui_t        = Ui::StationItemSettings;

 private:
 /*!
 \fn editingIP(QLineEdit* ipType, QString& value)
 \brief Function editing IP address
 \param[in] ipType pointer to the widget element
 \param[in] value pointer to value
*/
     void editingIP(QLineEdit* ipType, QString& value) noexcept;
/*!
 \fn setStation(Station const& station)
 \brief Station parameter display function 
 \param[in] station station parameters
*/
     void setStation(Station* station) noexcept;
     void initPretones() noexcept;
     void hidePretone() noexcept;  

 public:
/*!
 \fn StationItemSettings
 \brief Function creation class
*/
     StationItemSettings();
/*!
 \fn ~StationItemSettings()
 \brief Function destroy class
*/
     ~StationItemSettings();
 /*!
 \fn hideSettings(bool is)
 \brief Hiding interface elements function 
 \param[in] is flag
*/
     void hideSettings(bool is) noexcept;     
 /*!
 \fn setupUi(QWidget* parent, Station* station, StationItem* StationItem)
 \brief Function of customizing interface elements
 \param[in] parent pointer to the parent
 \param[in] station pointer to the station
 \param[in] StationItem pointer to the StationItem
*/
     void setupUi(QWidget* parent, Station* station, StationItem* StationItem);
 /*!
 \fn setKeySettingsEnabled(bool is, int countKeys = 0)
 \brief Control function of the ability to edit the parameters of the button 
 \param[in] is flag control
 \param[in] countKeys count keys
*/
     void setKeySettingsEnabled(bool is, int countKeys = 0) noexcept;
/*!
 \fn setKey(Key const& key)
 \brief Button parameter setting function 
 \param[in] key paramaters key
*/
     void setKey(Key const& key) noexcept;
/*!
 \fn setType(StationType const& type)
 \brief Button type station setting function 
 \param[in] key paramaters key
*/
	 void setType(StationType const& type) noexcept;
/*!
 \fn instanceSignals()
 \brief Station parameter change signal processing function 
*/
     void instanceStationSignals() noexcept;
/*!
 \fn instanceKeySignals()
 \brief Button parameter change signal processing function 
*/
     void instanceKeySignals() noexcept;
/*!
 \fn sendKey(uint priority, QString const& destination = {}, bool pretoneChanged = true)
 \brief Function of forming and transmitting the parameter of the button
 \param[in] priority priority connection 
 \param[in] destination cid destination
 \param[in] pretoneChanged flags change pretone
*/
     void sendKey(uint priority, QString const& destination = {}, bool pretoneChanged = true) noexcept;
/*!
 \fn retranslateUi()
 \brief The function of translating interface elements
*/
     void retranslateUi();
/*!
 \fn setSettingsPretone()
 \brief The function setting paramters pretone
*/
     void setSettingsPretone() noexcept;

 private slots:
/*!
 \fn pretoneChanged(const QString& pretone)
 \brief The function setting paramters pretone
 \param[in] pretone string change pretone
*/
     void pretoneChanged(const QString& pretone) noexcept;
/*!
 \fn pretonePlayChanged(const QString& pretone)
 \brief The function setting paramters pretone play
 \param[in] pretone string change pretone play
*/
     void pretonePlayChanged(QString const&) noexcept;
/*!
 \fn dataStationChanged()
 \brief station parameters change signal handler 
*/
     void dataStationChanged();
/*!
 \fn dataKeyChanged(QString const& destination = {})
 \brief key parameters change signal handler 
*/
     void dataKeyChanged(QString const& destination = {});
/*!
 \fn dataKeyChangedFlag(bool)
 \brief key parameters change signal handler flag
*/
     void dataKeyChangedFlag(bool);
/*!
 \fn functionChanged()
 \brief station function type change signal handler 
*/
     void functionChanged();

 signals:
/*!
 \fn textChanged(QString const& text)
 \brief text field change signal 
 \param[in] text string text field
*/
     void textChanged(QString const& text);
/*!
 \fn changedCid(cid_t cid)
 \brief cid change signal 
 \param[in] cid identificator station
*/
     void changedCid(cid_t cid);
/*!
 \fn keyChanged(Key& key, bool checkedPretone = false)
 \brief signal about changing the parameters of the button  
 \param[in] key parameters key
*/
     void keyChanged(Key& key, bool checkedPretone = false);
/*!
 \fn stationChanged(Station const& key)
 \brief signal about changing the parameters of the station  
 \param[in] key parameters station
*/
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
///pointer to parent module
     QWidget* m_parent = nullptr;
///IP address
     QString m_ip;
///serial number
	 QString m_sn;
///subnet mask
     QString m_mask;
///gateway
     QString m_gateway;
///cid identificator
     cid_t m_cid;
///identificator button
     uint m_id;
///type station
	 StationType m_type;
     DurationType m_savePretone;
     TracksModel* m_model = nullptr;
};

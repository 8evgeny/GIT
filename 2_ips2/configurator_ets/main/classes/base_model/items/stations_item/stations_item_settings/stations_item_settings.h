/*!
 * @file stations_item_settings.h
 * @file stations_item_settings.cpp
 * @brief This file contains a class for settings stations item
*/
#pragma once
#include <QtNetwork/QHostAddress>
#include "main/classes/base_model/items/stations_item/model_stations/station.h"
#include "main/classes/base_model/items/stations_item/model_stations/model_stations.h"

IPS_UI(StationsItemSettings)
/*! 
\defgroup stations_item_settings_module Class for configuring station parameters
*/
class StationsItemSettings final: public QWidget {
     Q_OBJECT
     IPS_DISABLE_COPY(StationsItemSettings)

 public:
     using class_name  = StationsItemSettings;
     using ui_t        = Ui::StationsItemSettings;

 private:
 /*!
 \fn editingIP(QLineEdit* ipType, QString& value)
 \brief Function editing IP address
 \param[in] ipType pointer to the widget element
 \param[in] value pointer to value
*/
    void editingIP(QLineEdit* ipType, QString& value) noexcept;
 /*!
 \fn editingHost(QLineEdit* ipType, QString& value)
 \brief Function editing host address
 \param[in] ipType pointer to the widget element
 \param[in] value pointer to value
*/
    void editingHost(QLineEdit* ipType, QString& value) noexcept;

 public:
/*!
 \fn StationsItemSettings
 \brief Function creation class
*/
     StationsItemSettings();
/*!
 \fn ~StationsItemSettings()
 \brief Function destroy class
*/
     ~StationsItemSettings();
 /*!
 \fn setupUi(QWidget* parent)
 \brief Function of customizing interface elements
 \param[in] parent pointer to the parent
*/
     void setupUi(QWidget* parent);
/*!
 \fn retranslateUi()
 \brief The function of translating interface elements
*/
     void retranslateUi();
/*!
 \fn instanceSignals()
 \brief Signal processing function
*/
     void instanceSignals() noexcept;
/*!
 \fn setStation(Station const& station)
 \brief Station parameter display function 
 \param[in] station station parameters
*/
     void setStation(Station const& station) noexcept;

 private slots:
/*!
 \fn dataStationChanged()
 \brief Station parameters change signal handler
*/
     void dataStationChanged();

 signals:
/*!
 \fn void stationChanged(Station const& key)
 \brief signal about changing station parameters  
 \param[in] key station parameters
*/
     void stationChanged(Station const& key);

 private:
     ui_t* ui = nullptr;
///IP address station
     QString m_ip;  
///Subnet mask station
     QString m_mask;
///Gateway station
     QString m_gateway;
///CID-identificator station
     cid_t m_cid;     
///pointer to the parent
     QWidget* m_parent = nullptr;     
};

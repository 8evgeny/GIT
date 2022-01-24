#pragma once
#include <QtNetwork/QHostAddress>
#include "main/classes/base_model/items/stations_item/model_stations/station.h"
#include "main/classes/base_model/items/stations_item/model_stations/model_stations.h"

IPS_UI(StationsItemSettings)

class StationsItemSettings final: public QWidget {
     Q_OBJECT
     IPS_DISABLE_COPY(StationsItemSettings)

 public:
     using class_name  = StationsItemSettings;
     using ui_t        = Ui::StationsItemSettings;

 private:
    void editingIP(QLineEdit* ipType, QString& value) noexcept;
    void editingHost(QLineEdit* ipType, QString& value) noexcept;

 public:
     StationsItemSettings();
     ~StationsItemSettings();     
     void setupUi(QWidget* parent);
     void retranslateUi();
     void instanceSignals() noexcept;
     void setStation(Station const& station) noexcept;

 private slots:
     void dataStationChanged();

 signals:
     void stationChanged(Station const& key);

 private:
     ui_t* ui = nullptr;
     QString m_ip;     
     QString m_mask;
     QString m_gateway;
     cid_t m_cid;     
     QWidget* m_parent = nullptr;     
};

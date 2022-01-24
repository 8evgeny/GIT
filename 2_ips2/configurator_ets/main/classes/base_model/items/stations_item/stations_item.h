/*!
 * @file stations_item.h
 * @file stations_item.cpp
 * @brief This file contains is a class that displays the parameters of the list stations
*/
#pragma once
#include "model_stations/model_stations.h"
#include "stations_item_settings/stations_item_settings.h"
#include "main/messages/dialogs/messages/messages_dialog.h"
#include "main/messages/model_tracks/tracks_model.h"

IPS_UI(StationsItem)
/*! 
\defgroup stations_item_module Class for configuring list stations
*/
class StationsItem final : public QWidget, public BaseItem {
    Q_OBJECT
    IPS_DISABLE_COPY(StationsItem)

    friend class TestStationItems;
    friend class TestValidator;

public:
     using class_name = StationsItem;
     using ui_t       = Ui::StationsItem;
     using inherited  = BaseItem;

public:
    signals:
    void changedPretones(QStringList const& pretones);

public:
/*!
 \fn createMenu()
 \brief menu creation function
*/
     void createMenu();
/*!
 \fn selectedRowDelete()
 \brief function to delete the selected station  
*/
     void selectedRowDelete() noexcept;
/*!
 \fn fillTypes(QMenu* menu = nullptr, QMenu* sub = nullptr)
 \brief function of creating a list of stations
*/
     void fillTypes(QMenu* menu = nullptr, QMenu* sub = nullptr);
/*!
 \fn sortTableStations()
 \brief button table sorting function  
*/
 	 void sortTableStations();

private:   
/*!
 \fn instanceSignals()
 \brief handler for the data change signal from the elements interface 
*/
     void installSignals();         
/*!
 \fn changeEvent(QEvent *event)
 \brief event handler 
 \param[in] event current event
*/
     void changeEvent(QEvent* event) override;
/*!
 \fn instanceConnectionToSettings()
 \brief handler for the data change signal from the module settings 
*/
     void instanceConnectionToSettings();
     bool deserializeTracks(QJsonObject const& obj) noexcept;
/*!
 \fn getMaxSupportKey(StationType type)
 \brief function of counting the maximum number of supported buttons based on the type of station
 \param[in] type current type station
 \return maximum number of supported buttons
*/
	 int getMaxSupportKey(StationType type);

private:
     template<typename T>
     void clearList(QList<T>& list) noexcept;

private slots:
/*!
 \fn instanceConnectionToSettings()
 \brief function of processing the parameters of the selected station 
*/
     void stationsSelectionChanged(const QItemSelection&, const QItemSelection&);
/*!
 \fn on_sectionClicked ( int logicalIndex,  Qt::SortOrder order )
 \brief sorting function of the list of buttons   
 \param[in] logicalIndex index field
 \param[in] order sorting method 
*/
	 void on_sectionClicked ( int logicalIndex,  Qt::SortOrder order );
/*!
 \fn stationChanged(Station const& station)
 \brief button data change signal handler  
*/
	 void stationChanged(Station const& station);
/*!
 \fn dataChanged(const QModelIndex& index, const QModelIndex&, QVector<int> const& roles)
 \brief button data change signal handler  
*/
	 void dataChanged(const QModelIndex& index, const QModelIndex&, QVector<int> const& roles);
	 
public:
     explicit StationsItem(BaseItem* item = nullptr);
     virtual ~StationsItem() override;

public:
/*!
 \fn show(QWidget* content, QWidget* set = {})
 \brief function of initialization of display of station parameters 
 \param[in] content pointer to the form of displaying the list of buttons 
 \param[in] set pointer to the form of displaying the parameters buttons 
*/
     virtual StationsItem* show(QWidget* content, QWidget* settings = {}) override;
/*!
 \fn deshow()
 \brief function of deinitialization of display of station parameters 
*/
     virtual void deshow() override;
     virtual bool error(BaseItem* Item = nullptr) const override;
     virtual QString name(BaseItem* Item = nullptr) const override;
/*!
 \fn serialize()
 \brief list stations parameter serialization function  
 \return json value serialization parameters
*/
     virtual QJsonValue serialize() const override;
/*!
 \fn deserialize(QJsonObject const& data)
 \brief list stations parameter deserialization function  
 \param[in] data json value parameters station
*/
     virtual bool deserialize(QJsonObject const& obj) final override;
/*!
 \fn clear()
 \brief function to clear the list of buttons   
*/
     virtual bool clear() final;

public:
     ModelStations* model() {
         return &m_model;
     }
     void setModelTones(TracksModel* modelTones) {
         m_modelTones = modelTones;
     }
     int currentRow() noexcept {
         return m_currentRow;
     }
     QStringList const& pretones() {
         return m_pretones;
     }

private:     
     ui_t* ui = nullptr;
     int m_currentRow = 0;
     QModelIndex m_index = {};
     ModelStations m_model;
	 QSortFilterProxyModel m_model_proxy;
     TracksModel* m_modelTones = nullptr;     
     StationsItemSettings m_settings;
     QStringList m_pretones = {};
     int m_countTracks = 4;
};

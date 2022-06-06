/*!
 * @file station_item.h
 * @file station_item.cpp
 * @brief This file contains is a class that displays the parameters of the current station 
*/
#pragma once
#include "model_keys/model_keys.h"
#include "classes/base_model/items/station_item/station_item_settings/station_item_settings.h"
#include "classes/base_model/items/groups/models/calls/group_calls/group_calls.h"
///maximum of supported buttons
#define MAX_COUNT_KEYS 48

IPS_UI(StationItem)

class GroupCall;
class StationItemSettings;
/*! 
\defgroup station_item_module Class for configuring station and list key parameters
*/
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
/*!
 \fn keysSelectionChanged(const QItemSelection&, const QItemSelection&)
 \brief function of processing changes in the parameters of the selected button 
*/
     void keysSelectionChanged(const QItemSelection&, const QItemSelection&);
/*!
 \fn tableview_customContextMenuRequested(const QPoint &)
 \brief button list menu processing function 
*/
	 void tableview_customContextMenuRequested(const QPoint &);
/*!
 \fn tableview_Clicked(const QModelIndex&)
 \brief the function of handling the change of the field of the list of buttons 
*/
	 void tableview_Clicked(const QModelIndex&);
/*!
 \fn tableview_doubleClicked(const QModelIndex&)
 \brief processing function of activating parameter change of the selected button 
*/
	 void tableview_doubleClicked(const QModelIndex&);
/*!
 \fn m_add_clicked()
 \brief "+" button handling function 
*/
	 void m_add_clicked();
/*!
 \fn m_rem_clicked()
 \brief "-" button handling function 
*/
	 void m_rem_clicked();
/*!
 \fn dataChanged(const QModelIndex& index, const QModelIndex&, QVector<int> const& roles)
 \brief button data change signal handler  
*/
	 void dataChanged(const QModelIndex& index, const QModelIndex&, QVector<int> const& roles);
/*!
 \fn changedCid(cid_t cidStation)
 \brief cid station change signal handler  
 \param[in] cidStation station parameters
*/
	 void changedCid(cid_t cidStation);
/*!
 \fn keyChanged(Key& key, bool checkedPretone = false)
 \brief key parameters change signal handler 
*/
	 void keyChanged(Key& key, bool checkedPretone = false);
/*!
 \fn stationChanged(Station const& station)
 \brief station parameters change signal handler 
*/
	 void stationChanged(Station const& station);

private:
/*!
 \fn instanceConnectionToSettings()
 \brief handler for the data change signal from the configuration module 
*/
     void instanceConnectionToSettings();
/*!
 \fn initDataFromTypeTable()
 \brief initialization data from table type 
*/
     void initDataFromTypeTable();
/*!
 \fn instanceSignals()
 \brief handler for the data change signal from the elements interface 
*/
     void instanceSignals();
/*!
 \fn changeEvent(QEvent *event)
 \brief event handler 
 \param[in] event current event
*/
     void changeEvent(QEvent *event) override;
     void setCurrentValuesStringToUI(const QModelIndex& index);
/*!
 \fn sortTableKey()
 \brief button table sorting function  
*/
	 void sortTableKey();
/*!
 \fn getMaxSupportKey(StationType type)
 \brief function of counting the maximum number of supported buttons based on the type of station
 \param[in] type current type station
 \return maximum number of supported buttons
*/
 	 int getMaxSupportKey(StationType type);

public:
/*!
 \fn show(QWidget* content, QWidget* set = {})
 \brief function of initialization of display of station parameters 
 \param[in] content pointer to the form of displaying the list of buttons 
 \param[in] set pointer to the form of displaying the parameters buttons 
*/
     virtual StationItem* show(QWidget* content, QWidget* set = {}) override;
/*!
 \fn deshow()
 \brief function of deinitialization of display of station parameters 
*/
     virtual void deshow() override;
     virtual bool error(BaseItem* Item = nullptr) const override;
     virtual QString name(BaseItem* Item = nullptr) const override;
/*!
 \fn serialize()
 \brief station parameter serialization function  
 \return json value serialization parameters
*/
     virtual QJsonValue serialize() const final;
/*!
 \fn deserialize(QJsonObject const& data)
 \brief station parameter deserialization function  
 \param[in] data json value parameters station
*/
     virtual bool deserialize(QJsonObject const& data) final override;
/*!
 \fn clear()
 \brief function to clear the list of buttons   
*/
     virtual bool clear() final;
/*!
 \fn DeleteKeys(int min, int max)
 \brief function to remove buttons from the list 
 \param[in] min minimum identifier button
 \param[in] max maximum identifier button
*/
	 void DeleteKeys(int min, int max);
/*!
 \fn setStationType(StationType type)
 \brief station type setting function 
 \param[in] type new type station
*/
	 void setStationType(StationType type){m_model.setStationType(type); m_type = type;};
/*!
 \fn insertKey(int row)
 \brief button insert function 
 \param[in] row button position
*/
	 void insertKey(int row);

public:
/*!
 \fn selectedRowDelete()
 \brief function to delete the selected button  
*/
     void selectedRowDelete() noexcept;

public:
/*!
\fn StationItem(BaseItem* item = nullptr)
\brief Function creation class
\param[in] item pointer to the parent class
*/
	explicit StationItem(BaseItem* item = nullptr);
/*!
 \fn ~StationItem()
 \brief Function destroy class
*/
     virtual ~StationItem() final;
     static class_name* defValue(BaseItem* parent, StationType type, uint cid, uint id, QString const& ip, QString const& mask, QString const& gateway, QString const& sn);
     static class_name* defValue(BaseItem* parent, uint cid, uint id, QString const& ip, QString const& mask, QString const& gateway, QString const& sn);
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
	 ModelKeys* getModel() {
		 return &m_model;
	 }

private slots:
     void updaterGroupsList(itemsGroups_t const&);
/*!
 \fn on_sectionClicked ( int logicalIndex,  Qt::SortOrder order )
 \brief sorting function of the list of buttons   
 \param[in] logicalIndex index field
 \param[in] order sorting method 
*/
	 void on_sectionClicked ( int logicalIndex,  Qt::SortOrder order );

private:
     QString m_hash;
     bool m_isChecked;
     ui_t* ui = nullptr;
     int m_row = -1;          
     App* m_app = nullptr;
	 QSortFilterProxyModel m_model_proxy;
     ModelKeys m_model;     
     QList<bool> m_defaultValues = {};     
     QStringList m_groupsList = {};
     StationItemSettings m_settings;
     TracksModel* m_modelTones = nullptr;
	 int currentMaxKey = MAX_COUNT_KEYS;
	 StationType m_type;
	 BaseItem* m_parent;
};

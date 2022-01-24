/*!
 * @file model_stations.h
 * @file model_stations.cpp
 * @brief This file contains station list view model class 
*/
#pragma once
#include "station.h"
#include "main/classes/base_model/items/station_item/station_item.h"

class StationsItem;
/*! 
\defgroup model_keys_module Button list view model class 
*/
class ModelStations final: public QAbstractTableModel {
    Q_OBJECT
    IPS_DISABLE_COPY(ModelStations)

    friend class TestStationItems;
    friend class TestStationItem;

public:
    using class_name = ModelStations;
    using inherited  = QAbstractTableModel;
    using items_t    = QList<StationItem*>;
    using item_t     = typename items_t::value_type;

private:
    signals:
    void removeStation(QString const& hash);

public:
    static const int SettingsRole = Qt::UserRole + 1;

private:
    QList<StationItem*> StationItems() const noexcept;
    QModelIndex stationsItemIndex() const noexcept;
    void updateCid(item_t item) noexcept;

public:
/*!
\fn ModelStations(QObject* parent = nullptr)
\brief Function creation class
\param[in] parent pointer to the parent class
*/
    explicit ModelStations(QObject* parent = nullptr);
/*!
 \fn ~ModelStations()
 \brief Function destroy class
*/
    ~ModelStations() = default;
/*!
 \fn rowCount(const QModelIndex &parent = {})
 \brief the function returns the number of rows
 \return number of columns
*/
    int rowCount(const QModelIndex& = {}) const final;
/*!
 \fn columnCount(const QModelIndex &parent = {})
 \brief the function returns the number of columns
 \return number of columns
*/
    int columnCount(const QModelIndex& parent = {}) const final;
    QVariant data(const QModelIndex& index, int role) const final;
    virtual bool setData(const QModelIndex& index, const QVariant &value, int role = Qt::EditRole) final;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const final;
/*!
 \fn serialize()
 \brief list stations parameter serialization function  
*/
    virtual QJsonArray serialize() const;
/*!
 \fn deserialize()
 \brief list stations parameter deserialization function  
*/
    virtual bool deserialize(QJsonArray const&) final;
/*!
 \fn insertRows(int position, int rows, const QModelIndex& index = {})
 \brief the function inserts new station
 \param[in] position insert position
 \param[in] rows number of stations
 \param[in] index current index model
 \return results inserts
*/
    bool insertRows(int position, int rows, const QModelIndex& = {}) final;    
/*!
 \fn removeRows(int position, int rows, const QModelIndex& index = {})
 \brief the function removes new station
 \param[in] position removes position
 \param[in] rows number of stations
 \param[in] index current index model
 \return results removes
*/
    bool removeRows(int position, int rows, const QModelIndex& = {}) final;
    Qt::ItemFlags flags(const QModelIndex &index) const final;
    bool insertItem(int position, StationItem* item,  QModelIndex const& index = {});
/*!
 \fn updateItem(int row, Station const& val, const QVector<int>& roles = {})
 \brief the function updates parameter station
 \param[in] row number of stations for update
 \param[in] val current parameters station
*/
    void updateItem(int row, Station const& val, const QVector<int>& roles = {}) noexcept;
/*!
 \fn station(int row)
 \brief the function return parameters selected station
 \param[in] row number of stations returns
 \return current parameters station
*/
    Station station(int row) const noexcept;
/*!
 \fn refreshStationItemName(int row)
 \brief the function updates station name
 \param[in] row number of stations for update
*/
    void refreshStationItemName(int row) const noexcept;
/*!
 \fn sort_serialize()
 \brief list stations parameter sorting serialization function  
*/
	QJsonArray sort_serialize();

public:
    void setParentStationItems(StationsItem* parent) {
        m_parent = parent;
    }
    StationsItem* parentStationItems() const {
        return m_parent;
    }
    items_t items() const noexcept {
        return StationItems();
    }
    void setModelTones(TracksModel* modelTones) noexcept {
        m_modelTones = modelTones;
    } 

private:        
    TracksModel* m_modelTones = nullptr;
    StationsItem* m_parent = nullptr;    

private:
/*!
 \fn getNumberAndPositionStations(int currentPos, int* newPos, int* Num)
 \brief the function returns new position and number station
 \param[in] currentPos current position into table
 \param[out] newPos pointer to new position station
 \param[out] Num pointer to new number station
 \return flags creating data
*/
	bool getNumberAndPositionStations(int currentPos, unsigned int* newPos, unsigned int* Num);
};

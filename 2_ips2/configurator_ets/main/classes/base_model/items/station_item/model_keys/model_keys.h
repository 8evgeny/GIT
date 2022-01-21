/*!
 * @file model_keys.h
 * @file model_keys.cpp
 * @brief This file contains button list view model class 
*/
#pragma once
#include "main/base_table.h"
#include "classes/base_types/station_types.h"
#include "key.h"

class StationItem;
/*! 
\defgroup model_keys_module Button list view model class 
*/
class ModelKeys final: public model::BaseTable<QList<Key>> {
    Q_OBJECT

 public:
    using class_name = ModelKeys;
    using keys_name  = Key;
    using inherited  = model::BaseTable<QList<Key>>;
    using items_t    = inherited::items_t;
    using value_t    = inherited::value_t;

 public:
/*!
\fn ModelKeys(QObject* parent = nullptr)
\brief Function creation class
\param[in] parent pointer to the parent class
*/
    explicit ModelKeys(QObject* parent = nullptr);
/*!
 \fn ~ModelKeys()
 \brief Function destroy class
*/
    ~ModelKeys() = default;
/*!
 \fn columnCount(const QModelIndex &parent = {})
 \brief the function returns the number of columns
 \return number of columns
*/
    int columnCount(const QModelIndex &parent = {}) const override;
/*!
 \fn defValue(uint id)
 \brief the function returns a pointer to the button name
 \param[in] id button id
 \return the button name
*/
    keys_name* defValue(uint id) const noexcept;
    QVariant data(const QModelIndex &index, int role) const override; 
/*!
 \fn insertRows(int position, int rows, const QModelIndex& index = {})
 \brief the function inserts new button
 \param[in] position insert position
 \param[in] rows number of buttons
 \param[in] index current index model
 \return results inserts
*/
    bool insertRows(int position, int rows, const QModelIndex& index = {}) override;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
/*!
 \fn serialize()
 \brief list keys parameter serialization function  
*/
    virtual QJsonArray serialize() const override;
/*!
 \fn deserialize(QJsonArray const&)
 \brief list keys parameter deserialization function  
*/
    virtual bool deserialize(QJsonArray const&) final override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
/*!
 \fn swapRow(int srcRowPos, int dstRowPos)
 \brief swap rows into table buttons 
 \param[in] srcRowPos source row
 \param[in] dstRowPos destination row
*/
	void swapRow(int srcRowPos, int dstRowPos);
/*!
 \fn idKey(int row)
 \brief the function returns a identificator button
 \param[in] row number of the row into table
 \return identificator button
*/
	int idKey(int row);
/*!
 \fn updateKey(int row, const Key&key)
 \brief the function update value button
 \param[in] row number of the row into table
 \param[in] key new button parameter
*/
	void updateKey(int row, const Key&key);
/*!
 \fn getCurrentRow()
 \brief the function return number of current row
 \return number of current row
*/
	int getCurrentRow();

 public:
/*!
 \fn setStationType(StationType type)
 \brief the function update type station
 \param[in] type new type station
*/
	 void setStationType(StationType type){stType = type;};
/*!
 \fn keys()
 \brief the function returns a list of keys
 \return list of keys
*/
    QList<Key> const& keys() const {
         return inherited::items();
    }
    void setParentStationItem(StationItem* parent) {
        m_parent = parent;
    }
    StationItem* parentStationItem() const {
        return m_parent;
    }
 
 private:
/*!
 \fn getNumberAndPositionKey(int currentPos, int* newPos, int* Num)
 \brief the function returns new position and number button
 \param[in] currentPos current position into table
 \param[out] newPos pointer to new position button
 \param[out] Num pointer to new number button
 \return flags creating data
*/
	bool getNumberAndPositionKey(int currentPos, int* newPos, int* Num);
/*!
 \fn bool getFreeNumberKey(bool _isDownFind, int* pNumber)
 \brief search function for the first free button number
 \param[in] _isDownFind search direction 
 \param[in] _startPos starting position 
 \param[out] newPos pointer to number button
 \return flags find number button
*/
	bool getFreeNumberKey(bool _isDownFind, int _startPos, int* _newPos, int* _pNumber);
/*!
 \fn unsigned int getMaximumKeyNumber()
 \brief the function returns maximum number button
 \return maximum number button
*/
	unsigned int getMaximumKeyNumber();
 private:
    StationItem* m_parent = nullptr;
	StationType stType;
	int m_row;
};

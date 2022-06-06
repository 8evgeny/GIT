#pragma once
#include <QWidget>
#include "classes/app.h"
#include "main_window.h"
#include "classes/base_model/base_model.h"
#include "include/validators/string_validator.h"
#include "classes/base_model/items/stations/station_item.h"
#include "classes/base_model/items/stations/station_items.h"


class TestStationItems {
public:
    TestStationItems(StationItems* item = nullptr)
        : m_testStItems(item)
    {
    }
public:
    bool insertRows(int position, int rows, const QModelIndex& index = {}) {
        return m_testStItems->m_model.insertRows(position, rows, index);
    }
    bool removeRows(int position, int rows, const QModelIndex& index) {
        return m_testStItems->m_model.removeRows(position, rows, index);
    }
    int rowCount() {
        return m_testStItems->m_model.rowCount();
    }
    void setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) {
        m_testStItems->m_model.setData(index, value, role);
    }
    bool setDataModel(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) {
        return m_testStItems->m_model.setData(index, value, role);
    }
    QVariant data(const QModelIndex &index, int role) {
        return m_testStItems->m_model.data(index, role);
    }
    QModelIndex indexModel(int row, int column, const QModelIndex &parent = QModelIndex()) {
        return m_testStItems->m_model.index(row, column, parent);
    }
    QJsonValue serializeModel() {
        return m_testStItems->m_model.serialize();
    }
    bool deserializeModel(QJsonArray const& arr) {
        return m_testStItems->m_model.deserialize(arr);
    }
    QJsonValue serializeModelChilds(int i) {
        return (dynamic_cast<StationItem*>(App::model()->StationItems()->child(i)))->serialize();
    }
    void insertRowsModelFromChilds(int i, int position, int rows, const QModelIndex& index) {
        auto typeStation = dynamic_cast<StationItem*>(App::model()->StationItems()->child(i));
             typeStation->m_model.insertRows(position, rows, index);
    }
    void setDataModelFromChilds(int i, const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) {
        auto typeStation = dynamic_cast<StationItem*>(App::model()->StationItems()->child(i));
             typeStation->m_model.setData(index, value, role);
    }
    QModelIndex indexModelFromChilds(int i, int row, int column, const QModelIndex &parent = QModelIndex()) {
        auto typeStation = dynamic_cast<StationItem*>(App::model()->StationItems()->child(i));
        return typeStation->m_model.index(row, column, parent);
    }
    QList<StationItem*> items() {
        return m_testStItems->m_model.StationItems();
    }
    StationItems* testStItems() {
        return m_testStItems;    
    }

private:
    StationItems* m_testStItems = nullptr;
};

class TestStationItem {
public:
    TestStationItem(StationItem* item = nullptr)
        : m_testStItem(item)
    {
    }
public:
    bool insertRows(int position, int rows, const QModelIndex& index = {}) {
        return m_testStItem->m_model.insertRows(position, rows, index);
    }
    int rowCount() {
        return m_testStItem->m_model.rowCount();
    }
    void setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) {
        m_testStItem->m_model.setData(index, value, role);
    }
    bool setDataModel(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) {
        return m_testStItem->m_model.setData(index, value, role);
    }
    QVariant data(const QModelIndex &index, int role) {
        return m_testStItem->m_model.data(index, role);
    }
    QModelIndex indexModel(int row, int column, const QModelIndex &parent = QModelIndex()) {
        return m_testStItem->m_model.index(row, column, parent);
    }
    QJsonValue serializeModel() {
        return m_testStItem->m_model.serialize();
    }
    bool deserializeModel(const QJsonArray& data) {
        return m_testStItem->m_model.deserialize(data);
    }
    QModelIndex indexModelFromChilds(int i, int row, int column, const QModelIndex &parent = QModelIndex()) {
        auto typeStation = dynamic_cast<StationItem*>(m_testStItem->items().value(i));
        return typeStation->m_model.index(row, column, parent);
    }
    QList<ActiveStation> itemsActivStations() {
        return m_testStItem->m_model.items();
    }    
    QList<MapPretones>& preton() {
        return m_testStItem->m_model.preton();
    }   
    QJsonValue serializeModelSettings() {
        return m_testStItem->m_modelSettings.serialize();
    }
    QList<PairsStations> itemsPairsStations() {
        return m_testStItem->m_modelSettings.pairsStations();
    }
    StationItem* testStItem() {
        return m_testStItem;
    }

private:
    StationItem* m_testStItem = nullptr;
};

class TestValidator {
public:
    explicit TestValidator(QWidget* ui = nullptr)
        : m_testUI(ui)        
    {
    }
    void setUI(QWidget* ui = nullptr) {
        m_testUI = ui;
    }
    QWidget* toUI() {
        return m_testUI;
    }
    void setValid(StringSizeValidator<5>* valid) {
        m_testValid = valid;
    }
    StringSizeValidator<5>* valid() {
        return m_testValid;
    }

private:    
    QWidget* m_testUI = nullptr;
    StringSizeValidator<5>* m_testValid = nullptr;    
};
class TestMainWindow {

public:
    explicit TestMainWindow(MainWindow* parent = nullptr)
        : m_testMain(parent) {
        }

private:
    MainWindow* m_testMain = nullptr;
};
class TestTransfer
{
public:
    TestTransfer();
};









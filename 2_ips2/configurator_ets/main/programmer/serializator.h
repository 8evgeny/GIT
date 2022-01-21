#pragma once
#include <QObject>
#include <QByteArray>
#include "model_programmer/model_programmer.h"
#include "main/classes/base_model/items/station_item/station_item.h"
#include "main/classes/base_model/items/stations_item/stations_item.h"
#include "classes/base_model/items/station_item/active_lines_serializator/active_lines_serializator.h"

class StationItem;
class StationItem;

class Serializator final : public QObject
{
    Q_OBJECT

    using class_name = Serializator;
    using inherited  = QObject;

    template<class T, class U>
    void appendTo(U u, QByteArray& arr) const {
        T value = static_cast<T>(u);
        arr.append(reinterpret_cast<char*>(&value), sizeof(T));
    }

public:
    QByteArray infoTable(StationItem* item) noexcept;
    QByteArray stations() noexcept;
    QByteArray keys(StationItem* item) noexcept;
    QByteArray info(StationItem* item) const noexcept;
    QByteArray appendPretones(uint data) const noexcept;
    QByteArray appendHost(QString const& data) const noexcept;
    QByteArray activateControlLines(StationItem* item) noexcept;

public:
    Serializator(ModelProgrammer* model, QObject* parent = nullptr);
    ~Serializator() = default;

private:
    ModelProgrammer* m_model =  nullptr;
    QList<StationProgrammer*> m_stations;
};

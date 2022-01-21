#include "serializator.h"
#include "model_programmer/model_programmer.h"
#include "classes/base_types/function_types.h"
#include "classes/base_types/station_types.h"
#include "classes/base_model/items/station_item/station_item_settings/citgit_helper.h"

Serializator::Serializator(ModelProgrammer* model, QObject* parent)
    :inherited(parent)
    , m_model(model)
{
}
QByteArray Serializator::appendHost(QString const& data) const noexcept
{
    QByteArray arr;
    QHostAddress host;
    host.setAddress(data);
    appendTo<std::uint32_t>(host.toIPv4Address(), arr);
    return arr;
}
QByteArray Serializator::appendPretones(uint data) const noexcept
{
    QByteArray arr;
    appendTo<std::uint32_t>(data, arr);
    return arr;
}
QByteArray Serializator::stations() noexcept
{
    QByteArray arr;
    appendTo<std::uint32_t>(m_model->rowCount(), arr);
    for (auto i = 0; i != m_model->rowCount(); ++i) {
         auto item = m_model->item(i)->stationItem();
         appendTo<std::uint32_t>(item->cid(), arr);
         appendHost(item->ip());
    }
    return arr;
}
QByteArray Serializator::info(StationItem* item) const noexcept
{
    QByteArray arr;
    arr.append(App::model()->toHash());
    appendHost(item->ip());
    appendHost(item->gateway());
    appendHost(item->stationMask());
    arr.append((item->name()).toStdString().c_str());
    return arr;
}
QByteArray Serializator::keys(StationItem* item) noexcept
{
    QByteArray arr;    
    arr.append(item->model().items().size(), sizeof(quint16));
    for (auto j = 0; j != item->model().items().size(); ++j) {
         auto key = item->model().items().value(j);
         auto func = key.function();
         auto dur = key.pretone().duration();
         appendPretones(key.id());
         appendPretones(static_cast<uint>(func));
         appendPretones(key.destination().toUInt());
         appendPretones(key.priority());
         appendPretones(static_cast<uint>(dur));
         appendPretones(key.pretone().id());
    }
    return arr;
}
QByteArray Serializator::activateControlLines(StationItem* item) noexcept
{
    ActiveLinesSerializator activLines;
    QByteArray arr;
    uint size_lines = 0;
    for (auto j = 0; j != item->model().items().size(); ++j) {
        auto key = item->model().items().value(j);
        if (!key.activateControlLines().isEmpty()) {
            ++size_lines;
            arr.append(activLines.serializeActiveLines(key.activateControlLines(), key.id()));
        }
        appendPretones(size_lines);
    }
    return arr;
}

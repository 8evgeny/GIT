#include "log_from_socket.h"

ModelLogs::ModelLogs(QObject* parent)
    :inherited(parent)
{
}
bool ModelLogs::insertRows(int position, int rows, const QModelIndex& index)
{
    if(rows < 1)
        return false;

    inherited::beginInsertRows(index, position, position + rows - 1);
    for (auto i = 0; i!= rows; ++i) {
        inherited::items().insert(position + rows - 1, m_error);
        m_error.clear();
    }
    inherited::endInsertRows();
    return true;
}
int ModelLogs::columnCount(const QModelIndex&) const
{
    return 1;
}
QVariant ModelLogs::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= inherited::items().size() || index.row() < 0)
        return {};

    if (role == Qt::DisplayRole) {
        return inherited::items().value(index.row());
    }
    return {};
}
Qt::ItemFlags ModelLogs::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    return Qt::ItemIsSelectable;
}
QJsonArray ModelLogs::serialize() const
{
    return {};
}
bool ModelLogs::deserialize(QJsonArray const&)
{
    return true;
}
void ModelLogs::update() noexcept
{
    this->removeRows(0, items().size());
    items().clear();
}

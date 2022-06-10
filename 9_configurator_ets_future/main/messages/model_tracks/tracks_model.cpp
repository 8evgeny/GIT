#include "include/log.h"
#include "include/typedefs.h"
#include "classes/program.h"
#include "tracks_model.h"
#include "main/classes/algo.h"
#include "main/classes/json.h"
#include <gt/model/base_table.h>
#include <QDirIterator>

TracksModel::TracksModel(QObject* parent)
    : inherited(parent)
{
    auto i=0;
    QDirIterator it(program::paths::sounds(), {IPS_STR("*.wav")}, QDir::Files | QDir::NoSymLinks | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
    while (it.hasNext()) {
        Track track(it.next());        
        track.setPretone(m_pretones.value(i));
        insertItem(rowCount(), track);
        ++i;
    }

    m_countDefaults = rowCount();
}
int TracksModel::columnCount(const QModelIndex&) const
{
    return COUNT_COLUMNS;
}
int TracksModel::rowCount(const QModelIndex&) const
{   
    return inherited::items().size();
}
Qt::ItemFlags TracksModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    return index.column() == 1
        ? Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable
        : Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}
bool TracksModel::setData(QModelIndex const& index, QVariant const& value, int role)
{
    if (!index.isValid() || index.row() >= rowCount() || index.row() < 0)
        return false;

    if (role != TrackIsPlayRole && role != TrackCheckedRole && role != Qt::EditRole)
        return false;

    for(auto& item : inherited::items())
       item.setIsPlay(false);

    auto& item = inherited::items()[index.row()];
    item.setIsPlay(value.toBool());

    if (role == TrackCheckedRole)
        item.setChecked(value.toBool());

    if (role == Qt::EditRole && index.column() == 1)
        item.setPath(value.toString());

    emit onUpdateView();
    return true;
}
QVariant TracksModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || index.row() >= rowCount() || index.row() < 0)
        return {};

    if (role == TrackIdRole)
        return index.row() + 1;

    auto item = inherited::items().value(index.row());
    if (role == TrackNameRole)
        return item.name();

    if (role == TrackPathRole)
        return item.path();

    if (role == TrackIsPlayRole)
        return item.isPlay();

    if (role == TrackCheckedRole)
        return item.isChecked();

    if (role == TrackHashRole)
        return item.toHash();

    if (role != Qt::DisplayRole)
        return {};

    if (index.column() == 1)
        return index.row() + 1;
    else if (index.column() == 2)
        return item.name();
    else if (index.column() == 3)
        return item.pretone();

    return {};
}
QVariant TracksModel::headerData(int section, Qt::Orientation orientation, int role) const
{
        if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
            switch (section) {
                case 0:
                    return iInfo::tr("Check");
                case 1:
                    return iInfo::tr("Number Track");
                case 2:
                    return iInfo::tr("Name");
                case 3:
                    return iInfo::tr("Duration");
                case 4:
                    return iInfo::tr("Motion");
            }
         }
    return {};
}
bool TracksModel::insertItem(int position, value_t const& value)
{
    QByteArray arr;
    arr.append(value.path().toStdString().c_str());
    QCryptographicHash hash(QCryptographicHash::Md5);
    hash.addData(arr);
    QString hashStr;
    QLatin1String hashLatin(hash.result());
    hashStr.append(hashLatin);
    value_t track = value;
    track.setHash(hashStr);
    return insertItems(position, 1, track);
}
QJsonArray TracksModel::serialize() const noexcept
{    
    QJsonArray res;
    auto items = inherited::items();
    if (!items.isEmpty())     {
        for (auto i=m_countDefaults; i < items.size(); ++i) {
            res.push_back(QJsonObject {
                {IPS_STR("track"), items.value(static_cast<int>(i)).path()}});
        }
    }
    return res;
}
bool TracksModel::deserialize(QJsonArray const& arrTracks) noexcept
{
    for (auto track : arrTracks) {
         auto const& itemTrack = track.toObject().value(IPS_STR("track")).toString();
         inherited::insertItem(rowCount(), Track(itemTrack));
    }
    emit onUpdateView();
    return true;
}

#include <QStringView>
#include "tracks_filter_model.h"
#include "tracks_model.h"

#include <QDebug>

TracksFilterModel::TracksFilterModel(QObject* parent)
    : QSortFilterProxyModel(parent)
{
}
void TracksFilterModel::setFilter(QString const& filter) noexcept
{
    m_filter = filter;
    invalidateFilter();
}
bool TracksFilterModel::filterAcceptsRow(int row, QModelIndex const& index) const
{
    auto const& indx = sourceModel()->index(row, 0, index);
    auto const is = m_filter.startsWith(QChar::fromLatin1('#'));

#if QT_VERSION > QT_VERSION_CHECK(5,15,0)
    QStringView filter = m_filter;
#else
    auto filter = m_filter;
#endif
    auto const role = is
            ? TracksModel::TrackIdRole
            : TracksModel::TrackNameRole;

    if(is)
        filter = m_filter.mid(1);

    auto const& name = sourceModel()->data(indx, role).toString();
    return name.contains(filter);
}

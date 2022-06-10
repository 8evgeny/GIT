#include "include/convert/string.h"
#include "model_comments.h"
#include "classes/json.h"
#include "classes/defines.h"

/** \namespace project_info */
namespace project_info {

ModelComments::ModelComments(QObject *parent)
    : inherited(parent)
{
}
int ModelComments::columnCount(const QModelIndex&) const
{
    return 3;
}
QVariant ModelComments::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= inherited::items().size() || index.row() < 0)
        return {};

    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        const auto& comment = inherited::item(index.row());

        if (index.column() == 0)
            return convert::toString(comment.dateTime());
        else if (index.column() == 1)
            return comment.author();
        else if (index.column() == 2)
            return convert::truncate(comment.comment(), 128);
    }
    return {};
}
QVariant ModelComments::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (section) {
            case 0:
                return iInfo::tr("Date");
            case 1:
                return iInfo::tr("Author");
            case 2:
                return iInfo::tr("Comment");
        }
     }
    return QVariant();
}
bool ModelComments::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid() || index.row() >= inherited::items().size() || index.row() < 0)
        return false;

    if (role == Qt::EditRole) {
        auto& comment = inherited::item(index.row());
        auto const& val = value.toString();

        if (index.column() == 0)
            comment.setDateTime(value.toDateTime());
        else if (index.column() == 1)
            comment.setAuthor(convert::truncate(val, 32));
        else if (index.column() == 2) {
            comment.setComment(convert::truncate(val, 128));
        }
        emit QAbstractItemModel::dataChanged(index, index, {Qt::EditRole});
    }
    return true;
}
QJsonArray ModelComments::serialize() const
{
    QJsonArray res;
    std::transform(inherited::items().cbegin(), inherited::items().cend(), std::back_inserter(res), [](const value_t& item) {
        return (QJsonObject{
            {IPS_STR("date"), item.dateTime().toString()}
          , {IPS_STR("author"), item.author()}
          , {IPS_STR("comment"), item.comment()}
         });
    });
    return res;
}
bool ModelComments::deserialize(QJsonArray const& arr)
{
    auto pos = 0;
    for(auto const& a: arr) {
        if (a.isUndefined() || !a.isObject())
            return false;

        value_t val;
        IPS_JSON_TO_STR(a.toObject(), IPS_STR("date"), date)
        val.setDateTime(QDateTime::fromString(date));

        IPS_JSON_TO_STR(a.toObject(), IPS_STR("author"), author)
        val.setAuthor(author);

        IPS_JSON_TO_STR(a.toObject(), IPS_STR("comment"), comment)
        val.setComment(comment);

        inherited::insertItems(pos++, 1, val);
    }
    return true;
}
} // end namespace project_info

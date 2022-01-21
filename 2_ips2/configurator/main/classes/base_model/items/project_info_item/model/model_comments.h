#pragma once
#include "comment.h"
#include "base_table.h"

/** \namespace project_info */
namespace project_info {

class ModelComments final: public model::BaseTable<QList<Comment>> {
    Q_OBJECT

 public:
    using class_name = ModelComments;
    using inherited  = model::BaseTable<QList<Comment>>;
    using items_t    = inherited::items_t;
    using value_t    = inherited::value_t;

 public:
    explicit ModelComments(QObject* parent = nullptr);
    ~ModelComments() = default;
    int columnCount(const QModelIndex &parent = {}) const final;
    QVariant data(const QModelIndex &index, int role) const final;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) final;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const final;
    virtual QJsonArray serialize() const final;
    virtual bool deserialize(QJsonArray const&) final override;
};
}  // end namespace project_info

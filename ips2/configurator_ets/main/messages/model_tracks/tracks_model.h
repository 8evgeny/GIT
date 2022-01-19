#pragma once
#include <gt/model/base_table.h>
#include "include/tr.h"
#include "track.h"
#include "classes/algo.h"
#include <QCryptographicHash>

#define COUNT_COLUMNS 5

class TracksModel final: public gt::model::BaseTable<QList<Track>> {
    Q_OBJECT

public:
    using class_name  = TracksModel;
    using track_name  = Track;
    using inherited   = gt::model::BaseTable<QList<Track>>;
    using items_t     = inherited::items_t;
    using value_t     = inherited::value_t;

public:
    static const int TrackNameRole   = Qt::UserRole + 1;
    static const int TrackPathRole   = Qt::UserRole + 2;
    static const int TrackIsPlayRole = Qt::UserRole + 3;
    static const int TrackIdRole     = Qt::UserRole + 4;
    static const int TrackHashRole   = Qt::UserRole + 5;
    static const int TrackCheckedRole = Qt::UserRole + 6;

public:
    explicit TracksModel(QObject* parent = nullptr);
    virtual ~TracksModel() = default;
    int rowCount(const QModelIndex& = {}) const final;
    int columnCount(const QModelIndex& parent = {}) const final;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const final;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const final;
    Qt::ItemFlags flags(const QModelIndex &index) const final;    
    virtual QJsonArray serialize() const noexcept;
    virtual bool deserialize(QJsonArray const&) noexcept;
    virtual bool setData(QModelIndex const& index, QVariant const& value, int role = Qt::EditRole) override final;
    virtual bool insertItem(int position, value_t const& value) override final;

public:
    int countDefaults() noexcept {
        return m_countDefaults;
    }

signals:
    void changedCountTracks(int count);
    void onUpdateView();

private:    
    int m_countDefaults = 0;
    QList<QString> m_pretones = {IPS_STR("00:01:00"),
                                 IPS_STR("00:00:59"),
                                 IPS_STR("00:00:59"),
                                 IPS_STR("00:01:00"),
                                 IPS_STR("00:01:00"),
                                 IPS_STR("00:01:00"),
                                 IPS_STR("00:01:00"),
                                 IPS_STR("00:01:00"),
                                 IPS_STR("00:01:00"),
                                 IPS_STR("00:01:00"),
                                 IPS_STR("00:01:00"),
                                 IPS_STR("00:01:00"),
                                 IPS_STR("00:01:00"),
                                 IPS_STR("00:01:00"),
                                 IPS_STR("00:01:00"),
                                 IPS_STR("00:01:00"),
                                 IPS_STR("00:01:00"),
                                 IPS_STR("00:01:00"),
                                 IPS_STR("00:01:00"),
                                 IPS_STR("00:01:00"),
                                 IPS_STR("00:01:00"),
                                 IPS_STR("00:01:00"),
                                 IPS_STR("00:01:00"),
                                 IPS_STR("00:01:00"),
                                 IPS_STR("00:01:00"),
                                 IPS_STR("00:01:00"),
                                 IPS_STR("00:01:00"),
                                 IPS_STR("00:01:00"),
                                 IPS_STR("00:01:00"),
                                 IPS_STR("00:01:00"),
                                 IPS_STR("00:01:00"),
                                 IPS_STR("00:01:00"),
                                };
};

#pragma once
#include <QString>
#include <QSortFilterProxyModel>

class TracksFilterModel final: public QSortFilterProxyModel {
    Q_OBJECT

 public:
    using class_name = TracksFilterModel;
    using inherited  = QSortFilterProxyModel;

 public:
    TracksFilterModel(QObject* parent = nullptr);
    void setFilter(QString const& filter) noexcept;

 protected:
    bool filterAcceptsRow(int row, QModelIndex const& index) const override;

 private:
    QString m_filter;
};

#pragma once
#include <QJsonArray>
#include "main/base_table.h"

class ModelLogs final : public model::BaseTable<QList<QString>> {
      Q_OBJECT
public:
   using class_name = ModelLogs;
   using inherited  = model::BaseTable<QList<QString>>;
   using items_t    = inherited::items_t;
   using value_t    = inherited::value_t;
   using error_t    = QString;

public:
   explicit ModelLogs(QObject* parent = nullptr);
   ~ModelLogs() = default;
   bool insertRows(int position, int rows, const QModelIndex& index = {}) override;
   int columnCount(const QModelIndex &parent = {}) const override;
   QVariant data(const QModelIndex &index, int role) const override;
   Qt::ItemFlags flags(const QModelIndex &index) const override;
   QJsonArray serialize() const override;
   bool deserialize(QJsonArray const&) final override;

public:
   void update() noexcept;
   void setError(error_t error) noexcept {
       m_error = error;
   }

private:
   error_t m_error = {};
};

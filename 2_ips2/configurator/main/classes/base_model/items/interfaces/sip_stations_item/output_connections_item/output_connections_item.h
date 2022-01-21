#pragma once
#include <QWidget>
#include "classes/base_model/items/base_item.h"
#include "include/typedefs.h"
#include "comment/comment.h"
#include "classes/base_model/items/interfaces/sip_stations_item/output_connections_item/model_output_connections/model_output_connections.h"

#define CID_COLUMN 0
#define NAME_COLUMN 1
#define NUMBER_COLUMN 2

IPS_UI(OutputConnectionsItem)

class OutputConnectionsItem final : public QWidget, public BaseItem {
    Q_OBJECT
    IPS_DISABLE_COPY(OutputConnectionsItem)

public:
     using class_name = OutputConnectionsItem;
     using ui_t       = Ui::OutputConnectionsItem;
     using inherited  = BaseItem;

private:  
     void changeEvent(QEvent* event) override;
     void instanceSignals() noexcept;

public:
     explicit OutputConnectionsItem(BaseItem* item = nullptr);
     virtual ~OutputConnectionsItem() override;

public:
     virtual OutputConnectionsItem* show(QWidget* content, QWidget* settings = {}) override;
     virtual void deshow() override;
     virtual bool error(BaseItem* Item = nullptr) const override;
     virtual QString name(BaseItem* Item = nullptr) const override;
     virtual QJsonValue serialize() const override;
     virtual bool deserialize(QJsonObject const& obj) final override;
     virtual bool clear() final;

public:
     ModelOutputConnections* model() noexcept {
         return &m_model;
     }

private:     
     int m_row = 0;
     ui_t* ui = nullptr;
     int m_currentRow = {};
     OutputComment m_comment;
     QModelIndex m_index = {};
     ModelOutputConnections m_model;
};

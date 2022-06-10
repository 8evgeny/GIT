#pragma once
#include <QWidget>
#include "classes/base_model/items/base_item.h"
#include "include/typedefs.h"
#include "classes/base_model/items/interfaces/sip_stations_item/input_connections_item/model_input_connections/model_input_connections.h"
#include "comment_settings/comment_settings.h"

#define INPUT_NUMBER_COLUMN 0
#define FUNCTION_COLUMN 1
#define DESTINATION_COLUMN 2
#define PRIORITY_COLUMN 3

IPS_UI(InputConnectionsItem)

class InputConnectionsItem final : public QWidget, public BaseItem {
    Q_OBJECT
    IPS_DISABLE_COPY(InputConnectionsItem)

public:
     using class_name = InputConnectionsItem;
     using ui_t       = Ui::InputConnectionsItem;
     using inherited  = BaseItem;

private:  
     void changeEvent(QEvent* event) override;
     void instanceSignals() noexcept;

public:
     explicit InputConnectionsItem(BaseItem* item = nullptr);
     virtual ~InputConnectionsItem() override;

public:
     virtual InputConnectionsItem* show(QWidget* content, QWidget* settings = {}) override;
     virtual void deshow() override;
     virtual bool error(BaseItem* Item = nullptr) const override;
     virtual QString name(BaseItem* Item = nullptr) const override;
     virtual QJsonValue serialize() const override;
     virtual bool deserialize(QJsonObject const& obj) final override;
     virtual bool clear() final;

private:     
     int m_row = 0;
     ui_t* ui = nullptr;
     int m_currentRow = 0;
     ModelInputConnections m_model;
     CommentSettings m_settings;
     QModelIndex m_index = {};
};

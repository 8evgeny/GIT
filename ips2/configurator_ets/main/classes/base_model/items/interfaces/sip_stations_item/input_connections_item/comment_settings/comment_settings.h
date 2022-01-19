#pragma once
#include <QObject>
#include "include/typedefs.h"
#include "comment_settings_instance.h"
#include "classes/base_types/function_types.h"
#include "classes/base_model/items/interfaces/sip_stations_item/input_connections_item/model_input_connections/input_connection.h"

IPS_UI(CommentSettings)

class CommentSettings final : public QObject
{
    Q_OBJECT
    IPS_DISABLE_COPY(CommentSettings)

public:
    using class_name  = CommentSettings;
    using ui_t        = Ui::CommentSettings;

public:
    signals:
    void inputNumberChanged(QString const&);
    void functionChanged(QString const&);
    void destinationChanged(QString const&);
    void priorityChanged(QString const&);
    void textChanged(QString const&);

private slots:
    void editData();
    void fildChanged() noexcept;

public:
   void setupUi(QWidget* parent);
   void updateUi() noexcept;
   void setInputConnection(InputConnection const& connnection) noexcept;
   QJsonValue serialize() const;
   bool deserialize(QJsonObject const& obj);
   void clear() noexcept;

private:   
   void instanceSignals() noexcept;

public:
    CommentSettings();
    ~CommentSettings();

private:
   FunctionType m_function;
   QString m_destination = {};
   QString m_priority = {};
   QString m_input_number = {};
   QString m_comment = {};
   ui_t* ui = nullptr;
};

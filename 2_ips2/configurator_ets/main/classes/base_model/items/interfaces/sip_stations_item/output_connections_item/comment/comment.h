#pragma once
#include <QWidget>
#include "include/typedefs.h"
#include "comment_instance.h"
#include "classes/base_model/items/interfaces/sip_stations_item/output_connections_item/model_output_connections/output_connection.h"

IPS_UI(Comment)

class OutputComment final: public QWidget {
     Q_OBJECT
     IPS_DISABLE_COPY(OutputComment)

 public:
     using class_name  = OutputComment;
     using ui_t        = Ui::Comment;

 public:
    signals:
    void oidChanged(QString const&);
    void nameChanged(QString const&);
    void numberChanged(QString const&);
    void textChanged(QString const&);

 public:
    void setupUi(QWidget* parent);
    void updateUi() noexcept;
    void setOutputConnection(OutputConnection const& connnection) noexcept;
    QJsonValue serialize() const;
    bool deserialize(QJsonObject const& obj);
    void clear() noexcept;

 private slots:
    void editData() noexcept;
    void fildChanged() noexcept;

 private:
    void instanceSignals() noexcept;

 public:
     OutputComment();
     ~OutputComment();

 private:
    ui_t* ui = nullptr;
    uint m_oid = 0;
    QString m_name = {};
    QString m_comment = {};
    int m_number = 0;
};

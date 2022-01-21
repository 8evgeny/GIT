#pragma once
#include <QWidget>
#include "include/typedefs.h"
#include "sip_settings_instance.h"

IPS_UI(SipSettings)

class SipSettings final: public QWidget {
     Q_OBJECT
     IPS_DISABLE_COPY(SipSettings)

 public:
     using class_name  = SipSettings;
     using ui_t        = Ui::SipSettings;

 public:
    signals:
     void checkBoxPosition(bool is);
     void prefixChanged(QString const&);
     void cidChanged(QString const&);

 public:
    void setupUi(QWidget* parent);
    QJsonValue serialize() const;
    bool deserialize(QJsonObject const& obj);
    void clear() noexcept;
    void visibleCid() noexcept;

 private slots:
    void editData() noexcept;

 private:
    void updateCid() noexcept;
    void instanceSignals() noexcept;
    void updateUi() noexcept;
    void setHash(QString const& hash) noexcept;
    QString const& toHash() noexcept;
    void createCid() noexcept;

 public:
     SipSettings();
     ~SipSettings();

 private:
     static inline bool m_isCshow = false;

 private:
    QString m_hash = {};
    QString m_prefix = {};
    QString m_cid = {};
    QString m_login = {};
    QString m_password = {};
    ui_t* ui = nullptr;
};

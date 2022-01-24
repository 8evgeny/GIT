#pragma once
#include "include/main_pch.h"
#include "main/classes/json.h"
#include "include/typedefs.h"

class IPSJsonWorker final {
 public:
     using class_name  = IPSJsonWorker;

 public:
    static QJsonValue extractMsg(const QByteArray& str) noexcept {
        QJsonParseError error;
        auto const& doc = json::load(QString::fromStdString(str.toStdString()), &error);
        if(error.error == QJsonParseError::NoError || error.error == QJsonParseError::IllegalValue) //!
           return doc.object().value(IPS_STR("msg"));
        return {};
    }
    static QJsonValue extractMsg(const QString& str) noexcept {
        return extractMsg(str.toUtf8());
    }
    static QString extractErrorMsg(const QByteArray& str) noexcept {
        auto const& msg = extractMsg(str);
        return msg.toObject({}).value(IPS_STR("err")).toString({});
    }
    static QJsonValue extractErrorMsg(const QString& str) noexcept {
        return extractErrorMsg(str.toUtf8());
    }
    static bool isErrorMsg(const QByteArray& str) noexcept {
        auto const& msg = extractErrorMsg(str);
        return !msg.isEmpty();
    }
    static bool isErrorMsg(const QString& str) noexcept {
        return isErrorMsg(str.toUtf8());
    }
};

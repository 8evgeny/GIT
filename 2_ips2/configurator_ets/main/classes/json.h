#pragma once
#include "include/log.h"

//!< begin namespace json
namespace json {

inline QString ser(QJsonObject const obj) {
    QJsonDocument doc(obj);
    auto const& arr = doc.toJson(QJsonDocument::Compact);
    return QLatin1String(arr);
}
inline QString ser(const QString& name, QJsonValue const obj) {
    return ser(QJsonObject{{name, obj}});
}
inline QJsonDocument load(QString const& file) {
    QFile json(file);
    json.open(QFile::ReadOnly);
    return QJsonDocument().fromJson(json.readAll());
}
inline QJsonDocument load(QString const& file, QJsonParseError* error) {
    QFile json(file);
    json.open(QFile::ReadOnly);
    return QJsonDocument().fromJson(json.readAll(), error);
}
inline bool save(QJsonDocument const& document, QString const& file) {    
    QFile json(file);
    json.open(QFile::WriteOnly);
    return json.write(document.toJson(QJsonDocument::Indented)) != 0;
}
inline QString getString(QJsonValue const& data, QString const& def) {
   if(data.isUndefined() || !data.isString()) {
       LOG_ERROR(false, "%1 is not found or invalid. Object is %2", data, ser(IPS_STR("error"), data));
       return def;
   }
    else
       return data.toString(def);
}
inline bool toString(QJsonValue const& data, QString* value) {
   if(data.isUndefined() || !data.isString())
       return LOG_ERROR(false, "%1 is not found or invalid. Object is %2", data, ser(IPS_STR("error"), data));
    else
       *value = data.toString();
    return true;
}
inline QString getString(QJsonObject const& data, QString const& name, QString const& def) {
   return getString(data.value(name), def);
}
inline bool toString(QJsonObject const& data, QString const& name, QString* value) {
   return toString(data.value(name), value);
}
inline bool toInt(QJsonValue const& data, int* value) {
   if(data.isUndefined() || !data.isDouble())
       return LOG_ERROR(false, "%1 is not found or invalid. Object is %2", data, ser(IPS_STR("error"), data));
    else
       *value = data.toInt();
    return true;
}
inline bool toInt(QJsonObject const& data, QString const& name, int* value) {
   return toInt(data.value(name), value);
}
inline bool toUint(QJsonValue const& data, uint* value) {
   if(data.isUndefined() || !data.isDouble())
       return LOG_ERROR(false, "%1 is not found or invalid. Object is %2", data, ser(IPS_STR("error"), data));
    else
       *value = static_cast<uint>(data.toInt());
    return true;
}
inline bool toUint(QJsonObject const& data, QString const& name, uint* value) {
   return toUint(data.value(name), value);
}
inline bool toArray(QJsonValue const& data, QJsonArray* value) {
   if(data.isUndefined() || !data.isArray())
       return LOG_ERROR(false, "%1 is not found or invalid. Object is %2", data, ser(IPS_STR("error"), data));
    else
       *value = data.toArray();
    return true;
}
inline bool toArray(QJsonObject const& data, QString const& name, QJsonArray* value) {
   return toArray(data.value(name), value);
}

} // end namespance json

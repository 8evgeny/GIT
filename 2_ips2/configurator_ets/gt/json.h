#pragma once
#include <initializer_list>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QString>
#include <QFile>
#include "typedefs.h"

//!< begin namespace gt::json
namespace gt::json {

inline QString ser(QJsonObject const obj) {
    QJsonDocument doc(obj);
    auto const& arr = doc.toJson(QJsonDocument::Compact);
    return QLatin1String(arr);
}
inline QString ser(const QString& name, QJsonValue const obj) {
    return ser(QJsonObject{{name, obj}});
}
inline QJsonDocument load(QFile& file, QJsonParseError* err = nullptr) {
    file.open(QFile::ReadOnly);
    return QJsonDocument().fromJson(file.readAll(), err);
}
inline QJsonDocument load(QByteArray const& arr, QJsonParseError* err = nullptr) {
    return QJsonDocument().fromJson(arr, err);
}
inline QJsonDocument load(QString const& str, QJsonParseError* err = nullptr) {
    return QJsonDocument().fromJson(str.toUtf8(), err);
}
inline bool save(QJsonDocument const& document, QString const& file) {
    QFile json(file);
    json.open(QFile::WriteOnly);
    return json.write(document.toJson(QJsonDocument::Indented)) != 0;
}
inline auto toString(QJsonValue const& data, QString const& def, bool* res = nullptr) {
    auto const is = (!data.isUndefined() && data.isString());
    if(res)
        *res = is;
    return data.toString(def);
}
inline auto toString(QJsonValue const& data, bool* res = nullptr) {
    return toString(data, {}, res);
}
inline auto toString(QJsonObject const& data, QString const& name, QString const& def, bool* res = nullptr) {
   return toString(data.value(name), def, res);
}
inline auto toString(QJsonObject const& data, QString const& name, bool* res = nullptr) {
    return toString(data, name, {}, res);
}
inline auto toInt(QJsonValue const& data, int def, bool* res = nullptr) {
    auto const is = (!data.isUndefined() && data.isDouble());
    if(res)
        *res = is;
    return data.toInt(def);
}
inline auto toInt(QJsonValue const& data, bool* res = nullptr) {
    return toInt(data, {}, res);
}
inline auto toInt(QJsonObject const& data, QString const& name, int def, bool* res = nullptr) {
   return toInt(data.value(name), def, res);
}
inline auto toInt(QJsonObject const& data, QString const& name, bool* res = nullptr) {
    return toInt(data, name, {}, res);
}
inline auto toUint(QJsonValue const& data, uint def, bool* res = nullptr) {
   return static_cast<uint>(toInt(data, static_cast<int>(def), res));
}
inline auto toUint(QJsonValue const& data, bool* res = nullptr) {
   return toUint(data, {}, res);
}
inline auto toUint(QJsonObject const& data, QString const& name, uint def, bool* res = nullptr) {
   return toUint(data.value(name), def, res);
}
inline auto toUint(QJsonObject const& data, QString const& name, bool* res = nullptr) {
   return toUint(data, name, {}, res);
}
inline auto toDouble(QJsonValue const& data, double def, bool* res = nullptr) {
    auto const is = (!data.isUndefined() && data.isDouble());
    if(res)
        *res = is;
    return data.toDouble(def);
}
inline auto toDouble(QJsonValue const& data, bool* res = nullptr) {
    return toDouble(data, {}, res);
}
inline auto toDouble(QJsonObject const& data, QString const& name, double def,  bool* res = nullptr) {
   return toDouble(data.value(name), def, res);
}
inline auto toDouble(QJsonObject const& data, QString const& name, bool* res = nullptr) {
   return toDouble(data, name, {}, res);
}
inline auto toBool(QJsonValue const& data, bool def, bool* res = nullptr) {
    auto const is = (!data.isUndefined() && data.isBool());
    if(res)
        *res = is;
    return data.toBool(def);
}
inline auto toBool(QJsonValue const& data, bool* res = nullptr) {
    return toBool(data, {}, res);
}
inline auto toBool(QJsonObject const& data, QString const& name, bool def, bool* res = nullptr) {
   return toBool(data.value(name), def, res);
}
inline auto toBool(QJsonObject const& data, QString const& name, bool* res = nullptr) {
   return toBool(data, name, {}, res);
}
inline auto toArray(QJsonValue const& data, QJsonArray const& def, bool* res = nullptr) {
    auto const is = (!data.isUndefined() && data.isArray());
    if(res)
        *res = is;
    return data.toArray(def);
}
inline auto toArray(QJsonValue const& data, bool* res = nullptr) {
    return toArray(data, {}, res);
}
inline auto toArray(QJsonObject const& data, QString const& name, QJsonArray const& def, bool* res = nullptr) {
   return toArray(data.value(name), def, res);
}
inline auto toArray(QJsonObject const& data, QString const& name, bool* res = nullptr) {
   return toArray(data, name, {}, res);
}
inline auto toObject(QJsonValue const& data, QJsonObject const& def, bool* res = nullptr) {
    auto const is = (!data.isUndefined() && data.isObject());
    if(res)
        *res = is;
    return data.toObject(def);
}
inline auto toObject(QJsonValue const& data, bool* res = nullptr) {
    return toObject(data, {}, res);
}
inline auto toObject(QJsonObject const& data, QString const& name, QJsonObject const& def, bool* res = nullptr) {
   return toObject(data.value(name), def, res);
}
inline auto toObject(QJsonObject const& data, QString const& name, bool* res = nullptr) {
   return toObject(data, name, {}, res);
}

} // end namespance gt::json

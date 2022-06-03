#pragma once
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkAccessManager>
#include "include/typedefs.h"
#include "include/network/json_worker.h"
#include "include/log.h"
#include <functional>
#include <gt/json.h>

class IPSHttp final {

 public:
    using class_name = IPSHttp;
    using confunc_t  = std::function<void(QString const&)>;
    using func_t     = std::function<void(QString const&, QJsonValue const&)>;
    using tuple_t    = std::tuple<QString, int, QString>;

    IPSHttp() = default;
    ~IPSHttp() = default;

 public:
  static void setUrl(const QString& url) noexcept {
          auto location = (!url.startsWith(IPS_STR("http://")))
              ? IPS_STR("http://") + url
              : url;
          m_url = location + QLatin1Char('/');
 }
 static int userId() noexcept {
        return m_uid;
 }
 static QString const& ipsUrl() noexcept {
        return m_url;
 }
 static tuple_t pack() noexcept {
     return std::make_tuple(m_url, m_uid, m_token);
 }
 static void unpack(tuple_t const& val) noexcept {
       m_url = std::get<0>(val);
       m_uid = std::get<1>(val);
       m_token = std::get<2>(val);
 }
 /**
   * \code
        IPSHttp::get(GT_STR("v1/recorders/items"), [&value](QString const& err, QJsonValue const& data){
            if (!err.isEmpty())
                MESSAGE_BOX_ERROR(err);
            else
                qDebug() << data;
        });
     *  \endcode
    **/  
 /**
  * \code
     IPSHttp::del(QString::fromUtf8("v1/recorder/group/"), [&value](QString const& err, QJsonValue const& data){
         if (!err.isEmpty())
             MESSAGE_BOX_ERROR(err);
         else
             qDebug() << data;
     });
  *  \endcode
 **/
 static void get(const QString& location, func_t fun) {
         //LOG_REGUEST(false, "GET: %1", location);
         IPSHttp::exec(fun)->get(IPSHttp::createLocation(location));
 }
 static void post(const QString& location, const QByteArray& body, func_t fun) {
         //LOG_REGUEST(false, "POST: %1 | %2", location, body);
         IPSHttp::exec(fun)->post(IPSHttp::createLocation(location), body);
 }
 static void put(const QString& location, const QByteArray& body, func_t fun) {
         //LOG_REGUEST(false, "PUT: %1 | %2", location, body);
         IPSHttp::exec(fun)->put(IPSHttp::createLocation(location), body);
 }
 static void del(const QString& location, func_t fun) {
         //LOG_REGUEST(false, "DELETE: %1", location);
         IPSHttp::exec(fun)->deleteResource(IPSHttp::createLocation(location));
 }
 static QNetworkRequest createLocation(const QString& location) noexcept {
         auto request = QNetworkRequest(QUrl(m_url + location));
         request.setHeader(QNetworkRequest::ContentTypeHeader, IPS_STR("application/json"));
         request.setRawHeader(QByteArray("Accept"), QByteArray("application/json"));
         if (!m_token.isEmpty())
             request.setRawHeader("Authorization", IPS_STR("Bearer %1").arg(m_token).toUtf8());
         return request;
 }
 static QNetworkAccessManager* exec(func_t fun) {
            auto manager = new QNetworkAccessManager;
            QObject::connect(manager, &QNetworkAccessManager::finished, [fun, manager](QNetworkReply* reply) {
                auto const is = reply->error() == QNetworkReply::NoError;
                auto const& data = reply->readAll();

                if (is) {
                    auto msg = IPSJsonWorker::extractMsg(data);
                    if (msg.isUndefined()) {
                        fun(IPS_STR("Cannot extract 'msg' from | %1").arg(QString::fromUtf8(data)), {});
                        //LOG_RESPONSE(false, "Cannot extract 'msg' from | %1", data);
                    }
                    else {
                        //LOG_RESPONSE(false, "OK %1", msg);
                        fun({}, msg);
                    }
                }
                else {
                    auto err = IPSJsonWorker::extractErrorMsg(data);
                    if (err.isEmpty()) {
                        //LOG_RESPONSE(false, "Error %1", reply->errorString());
                        fun(reply->errorString(), {});
                    }
                    else {
                        //LOG_RESPONSE(false, "Error %1", err);
                        fun(err, {});
                    }
                }
                reply->close();
                reply->deleteLater();
                manager->deleteLater();
            });
            return manager;
 }
 void connect(const QString& url, const QString& login, const QString& pass, confunc_t fun) noexcept {
        setUrl(url);

        QJsonObject obj = {
             {IPS_STR("login"), login}
           , {IPS_STR("pass"), pass}
        };
        exec([fun](QString const& err, QJsonValue const& data) {

            if(err.isEmpty()){
                IPSHttp::m_token = data.toObject({}).value(IPS_STR("token")).toString({});
                IPSHttp::m_uid = data.toObject({}).value(IPS_STR("id")).toInt({});
                if(IPSHttp::m_token.isEmpty())
                    return fun(IPS_STR("Invalid token | %1").arg(gt::json::ser(IPS_STR("msg"), data)));
                if(IPSHttp::m_uid < 1)
                    return fun(IPS_STR("Invalid uid | %1").arg(gt::json::ser(IPS_STR("msg"), data)));
            }
            fun(err);
        })->post(IPSHttp::createLocation(IPS_STR("v1/login")), gt::json::ser(obj).toUtf8());
    }

 private:
   static inline QString m_url;
   static inline int m_uid;
   static inline QString m_token;   
};

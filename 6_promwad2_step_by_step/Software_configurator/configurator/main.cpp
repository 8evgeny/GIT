#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "appcore.h"
#include <thread>
#include <QObject>
#include <QJsonObject>
#include <QJsonDocument>
#include "RuLang.h"
#include "RuTranslator.h"

#define BOOST_SPIRIT_THREADSAFE
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>

#include <algorithm>
#include <boost/filesystem.hpp>
#include <fstream>
#include <vector>

#include <QtCore/QCoreApplication>
#include <QtCore/QDebug>
#include <QtCore/QFile>
#include <QtCore/QStringList>
#include <QtCore/QVariant>
#include <QtCore/QTextCodec>
#include <QtCore/QTime>

#include "Lib/qjson/include/QJson/Parser"
#include <Lib/qjson/include/QJson/Serializer>
#include "Lib/qjson/src/cmdlineparser.h"

#include <QTest>

#include "receiver.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    // Create the receiver object
    Receiver receiver;

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    AppCore appCore;
    QString version;
//    QFile file("../configurator/versions_configurator");
//    file.open(QIODevice::ReadOnly);
//    QByteArray ba = file.readLine();
//    ba.append(file.readLine());
//    ba.append(file.readLine());
//    QString baStr = QString(ba);
//    QRegExp rx("(\\d+)");
//    QStringList list;
//    int pos = 0;
//    while ((pos = rx.indexIn(baStr, pos)) != -1)
//    {
//        list << rx.cap(1);
//        pos += rx.matchedLength();
//    }
//    version = list[0] + "." + list[1]+ "." + list[2];
//    qDebug() << "version: " << version;


    app.setOrganizationName("GIT");
    app.setOrganizationDomain("GIT");
    app.setApplicationName("Software configurator (version "+version+")");
    app.setApplicationVersion(version);
    qmlRegisterType<RuLang>("RuLang", 1, 0, "RuLang");
    RuTranslator mTrans(&app);

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
    &app, [url](QObject * obj, const QUrl & objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);


    QQmlContext *context = engine.rootContext();
    context->setContextProperty("appCore", &appCore);
    context->setContextProperty("rusTrans", &mTrans);
    context->setContextProperty("receiver", &receiver);

    engine.load(url);

    return app.exec();
}

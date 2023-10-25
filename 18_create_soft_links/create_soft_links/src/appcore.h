/*!
 \file appcore.h

*/

#ifndef APPCORE_H
#define APPCORE_H

#include <QObject>
#include <QString>
#include <QUrl>
#include <QTextStream>
#include <QTimer>
#include <QQueue>
#include <QStack>
#include <QtNetwork>
#include <QColor>
#include "main.h"

class AppCore : public QObject
{

    Q_OBJECT
public:

    explicit AppCore(QObject *parent = nullptr);

signals:
    //! Signal to transmit data to interface qml-interface

    void exitFromMenu();
    void closeMainWindow();
    void windowChangesBeforeSave();
    void windowChangesMenuSave();


public slots:
    //! The slot for the receiving of data from the QML-interface

//    void encryptionBinFile(const QUrl &pathFile,  const QString &key, const QString &dateTime);
//    void updateAllViews();
//    QByteArray strHex(const std::string &data);
    void needCloseMainWindow();
    void exitActionFromMenu();
    void readConfig(const char* conf_file);
private:
    std::map<std::string, std::string> config;

};
#endif // APPCORE_H

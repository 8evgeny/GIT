#include <QCoreApplication>
#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QDebug>

#ifdef Q_OS_WIN32
    #define SYS_CODEC "CP1251"
    #define SYS_TERM_CODEC "866"
#elif defined Q_OS_UNIX
    #define SYS_CODEC "UTF-8"
    #define SYS_TERM_CODEC "UTF-8"
#endif

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
//    QFile file("../index.json");
    QFile file("../example.json");
    file.open(QIODevice::ReadOnly);
    QString jsonData = file.readAll();
    file.close();
    QByteArray ba = jsonData.toUtf8();
//    QByteArray ba = QByteArray::fromRawData(jsonData.toStdString().c_str(),sizeof (jsonData.toStdString()));

    QJsonDocument jsonDocument = QJsonDocument::fromJson(ba);
    QJsonObject jsonObject = jsonDocument.object();
    QString FirstName = jsonObject["FirstName"].toString();

    qDebug() << FirstName;

    a.exit(0);
}

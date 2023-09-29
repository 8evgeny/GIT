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

    QString val;
    QFile file;
    file.setFileName("../index.json");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    val = file.readAll();
    file.seek(0);
    QByteArray data = file.readAll();
    file.close();

    QJsonDocument jsonDocument(QJsonDocument::fromJson(QString(data)));
    QJsonDocument doc = QJsonDocument::fromJson(val.toUtf8());
 QJsonObject json1 = jsonDocument.object();
    QJsonObject json = doc.object();


    QString  naim = "Наименовани";
    QString NaimenovanieIzdeliya = json[naim].toString();
    int numberPages = json["Общее количество листов документа"].toInt();
    qDebug() << NaimenovanieIzdeliya;
    qDebug() << numberPages;
    return a.exec();
}

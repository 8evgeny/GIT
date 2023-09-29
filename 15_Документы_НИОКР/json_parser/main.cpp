#include <QCoreApplication>
#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

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
    file.close();
    QJsonDocument doc = QJsonDocument::fromJson(val.toUtf8());


    QJsonObject json = doc.object();
    QString NaimenovanieIzdeliya = json["Наименование изделия"].toString();
    int numberPages = json["Общее количество листов документа"].toInt();
    qDebug() << NaimenovanieIzdeliya;
    qDebug() << numberPages;
    return a.exec();
}

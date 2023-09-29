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
    // Step 1: Read the JSON file into a QByteArray.
    QFile file("../index.json");
    file.setFileName("../index.json");
    file.open(QIODevice::ReadOnly);
    QByteArray jsonData = file.readAll();
    file.close();

    // Step 2: Create a QJsonDocument using the QByteArray.
    QJsonDocument jsonDocument = QJsonDocument::fromJson(jsonData);

    // Step 3: Extract the QJsonObject from the QJsonDocument.
    QJsonObject jsonObject = jsonDocument.object();

    // Access the values in the QJsonObject.
    QString name = jsonObject["Наименовани"].toString();
    int age = jsonObject["age"].toInt();
    QString city = jsonObject["city"].toString();



//    QJsonDocument doc = QJsonDocument::fromJson(val.toUtf8());
// QJsonObject json1 = jsonDocument.object();
//    QJsonObject json = doc.object();


//    QString  naim = "Наименовани";
//    QString NaimenovanieIzdeliya = json[naim].toString();
//    int numberPages = json["Общее количество листов документа"].toInt();
//    qDebug() << NaimenovanieIzdeliya;
//    qDebug() << numberPages;
    return a.exec();
}

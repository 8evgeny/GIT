#include "main.h"

int main(int argc, char *argv[])
{
    QFile file("../index.json");
    file.open(QIODevice::ReadOnly);
    QString jsonData = file.readAll();
    file.close();
    parseJSON(jsonData);

    file.setFileName("../index1.json");
    file.open(QIODevice::ReadOnly);
    QString jsonData1 = file.readAll();
    file.close();
    parseJSON(jsonData1);

}

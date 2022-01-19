#include "classes/app.h"
#include "main_window.h"
#include "include/typedefs.h"
#include "classes/base_model/items/stations_item/stations_item.h"

void addStyle() {
    QFile styleF(IPS_STR(":/css/style.css"));
    styleF.open(QFile::ReadOnly);
    qApp->setStyleSheet(QString::fromStdString(styleF.readAll().toStdString()));
}
inline void initPaths()
{
    QFileInfo file{program::logFile()};
    if(!file.exists())
        QDir().mkpath(file.absoluteDir().absolutePath());
}

int main(int argc, char *argv[])
{
    App a(argc, argv);
    addStyle();
    initPaths();

    MainWindow w;
    w.showFullScreen();
    return a.exec();
}

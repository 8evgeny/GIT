#include "main.h"
#include "widget.h"
#if 0

 #endif
#include <QApplication>
#include <QListWidget>

int main(int argc, char *argv[])
{
    QApplication app( argc, argv );

    Widget w;
    w.setWindowTitle("Создание комплекта документов на изделие");
    w.show();

return app.exec();
}

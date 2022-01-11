#include "mainwindow.h"
#include <QApplication>
#include <qapplication.h>
#include <stdio.h>
#include <stdlib.h>
#include <QDebug>
#include <QSettings>

/*** https://evileg.com/ru/post/154/
 *   http://pavelk.ru/qt-nastraivaem-logirovanie-v-proekte-format-loga
 * sptintf, fprintf
 ***/
void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QByteArray localMsg = msg.toLocal8Bit();
		const char *filename = context.file+13; // cut "../updater71/"
    switch (type) {
    case QtDebugMsg:
        fprintf(stderr, "Debug: %s (%s:%u, %s)\n", localMsg.constData(), filename, context.line, context.function);
        break;
    case QtInfoMsg:
        fprintf(stderr, "Info: %s (%s:%u, %s)\n", localMsg.constData(), filename, context.line, context.function);
        break;
    case QtWarningMsg:
        fprintf(stderr, "Warning: %s (%s:%u, %s)\n", localMsg.constData(), filename, context.line, context.function);
        break;
    case QtCriticalMsg:
        fprintf(stderr, "Critical: %s (%s:%u, %s)\n", localMsg.constData(), filename, context.line, context.function);
        break;
    case QtFatalMsg:
        fprintf(stderr, "Fatal: %s (%s:%u, %s)\n", localMsg.constData(), filename, context.line, context.function);
        abort();
    }
		fflush(stderr);
}

QCoreApplication* createApplication(int &argc, char *argv[])
{
    for (int i = 1; i < argc; ++i)
        if (!qstrcmp(argv[i], "-no-gui"))
            return new QCoreApplication(argc, argv);
    return new QApplication(argc, argv);
}

int main(int argc, char *argv[])
{
    /* TODO: что хочется добавить в проект: (проверить ВЕ3)
     * 1. запуск с параметрами
     * 2. работу с qesp
     * 3. с доработкой загрузчика:
     *    - частичное стирание флешки
     *    - работа с инфо областью
     * 4. формат bin для загрузки файла
     * 5. Проверить, что с ВЕ3 не будет той же проблемы, что и с ВЦ1!
     * */
    //QString qFormatLogMessage(QtMsgType type, const QMessageLogContext & context, const QString & str)
    qInstallMessageHandler(myMessageOutput);
    QScopedPointer<QCoreApplication> app(createApplication(argc, argv));
    
    qRegisterMetaType<MdrLoader::WorkType>("MdrLoader::WorkType");
    QCoreApplication::setOrganizationName("prostoRoman");
    QCoreApplication::setOrganizationDomain("prostoRoman.com");
    QCoreApplication::setApplicationName("MdrFwUart");
    QSettings::setDefaultFormat(QSettings::IniFormat);
    
    QApplication *a(qobject_cast<QApplication *>(app.data()));
    if (a) {
       // start GUI version...
        //qDebug()<< a->font();
        MainWindow w;
        w.show();
        return app->exec();
    } else {
       // start non-GUI version...
    }
    return -1;
}

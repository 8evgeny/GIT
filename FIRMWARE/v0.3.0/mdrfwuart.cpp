
#include <stdio.h>
#include <stdlib.h>
#include <QApplication>
#include <QDateTime>
#include <QDebug>
#include <QFileDialog>
#include <QFileSystemModel>
#include <QMessageBox>
#include <QSerialPortInfo>
#include <QSettings>
#include <QTextCodec>

#include "mdrfwuart.h"
#include "ui_mdrfwuform.h"
#include "bytearray.h"

//#ifdef Q_OS_LINUX
//static const QTextCodec *codec = QTextCodec::codecForName("UTF-8");
//#else //Q_OS_WIN
//static const QTextCodec *codec = QTextCodec::codecForName("IBM866");
//#endif

#define U QString::fromUtf8

MdrFwUart::MdrFwUart(QString f, QWidget *parent) :
    QMainWindow(parent),
    fwfnParam(f),
    mdrLoader(new VasiliLoaderClient),
    ui(new Ui::MdrFwUForm)
{
    ui->setupUi(this);
    setWindowTitle(U("MdrFwUart v.%1.%2.%3")
                   .arg(VER_MAJOR).arg(VER_MINOR).arg(VER_PATCH));
    
    ui->flasherfnLE->hide();
    ui->flasherfnPB->hide();
    
    ui->mcuSelectCB->addItem(U("Автоопределение"), QVariant(Mcu::UndefinedMcu));
    ui->mcuSelectCB->addItem(U("1901ВЦ1 / MDR32FG16S1"), QVariant(Mcu::MDR1901VC1));
    ui->mcuSelectCB->addItem(U("1986ВЕ1 / MDR32F1"), QVariant(Mcu::MDR1986BE1));
    ui->mcuSelectCB->addItem(U("1986ВЕ3"), QVariant(Mcu::MDR1986BE3));
    ui->mcuSelectCB->addItem(U("1986ВЕ4"), QVariant(Mcu::MDR1986BE4));
    ui->mcuSelectCB->addItem(U("1986ВЕ9x / MDR32F9Q2"), QVariant(Mcu::MDR1986BE9X));
    
    updateComPortCB();
    
    mdrLoader->moveToThread(&loaderThread);
    loaderThread.start();
    
//#if (QT_VERSION >= QT_VERSION_CHECK(5, 2, 0))
// можно делать на лямбдах и прочее, например
//connect(&loaderThread, &QThread::finished, mdrLoader, &QObject::deleteLater);
//connect(this, &MainWindow::operate, mdrLoader, &VasiliLoaderClient::doWorkNew);
//connect(mdrLoader, &VasiliLoaderClient::statusChanged, ui->sL, &QLabel::setText);
//connect(mdrLoader, &VasiliLoaderClient::statusChanged, this, &MainWindow::logStatus);
//#else
    connect(&loaderThread, SIGNAL(finished()), mdrLoader, SLOT(deleteLater()));
    connect(this,      SIGNAL(operate(VasiliLoaderClient::WorkType)),
            mdrLoader, SLOT(doWorkNew(VasiliLoaderClient::WorkType)));
    connect(mdrLoader, SIGNAL(statusChanged(QString)), ui->sL, SLOT(setText(QString)));
    connect(mdrLoader, SIGNAL(statusChanged(QString)), this, SLOT(logStatus(QString)));
    connect(mdrLoader, SIGNAL(textChanged(QString)), ui->t, SLOT(appendPlainText(QString)));
    connect(mdrLoader, SIGNAL(textChanged(QString)), this, SLOT(logText(QString)));
    connect(mdrLoader, SIGNAL(dumpFilenameChanged(QString)), ui->dumpfnLE, SLOT(setText(QString)));
    connect(mdrLoader, SIGNAL(stopButtonSetEnable(bool)), ui->startStopPB, SLOT(setEnabled(bool)));
    connect(mdrLoader, SIGNAL(workFinished()), SLOT(loaderFinished()));
    connect(mdrLoader, SIGNAL(workFinished()), SLOT(updateComPortCB()));
//#endif
    readSettings();
}

MdrFwUart::~MdrFwUart()
{
    loaderThread.quit();
    loaderThread.wait();
    delete ui;
}

void MdrFwUart::closeEvent(QCloseEvent *event)
{
    if (/* DISABLES CODE */ (true) /*userReallyWantsToQuit()*/ ) {
        loaderThread.quit();
        loaderThread.wait();
        writeSettings();
        event->accept();
    } else {
        event->ignore();
    }
}

void MdrFwUart::on_startStopPB_clicked()
{
    bool ok;
    
    if(ui->startStopPB->isChecked() == false) {
      ui->t->appendHtml(U("<font color=\"red\"><b>"
                          "Принудительно остановлено пользователем."
                          "</font></b>"));
        mdrLoader->stopWork();
        return;
    }
    if(ui->dumpfnLE->text().isEmpty()) {
        QString fn(qApp->applicationDirPath()+QDir::separator());
        fn = QDir::toNativeSeparators(fn);
        fn += QDateTime::currentDateTime().toString("yyMMdd_hhmmss");
        ui->dumpfnLE->setText(fn);
    }
    ui->startStopPB->setText("STOP!");
    ui->startStopPB->setDisabled(true);
    ui->t->clear();

    mdrLoader->setPortName(ui->portNameCB->itemData(
                             ui->portNameCB->currentIndex()).toString());
    mdrLoader->setBaudRate(
          (QSerialPort::BaudRate)ui->portBaudrateCB->currentText().toUInt(&ok));
    mdrLoader->setDumpFilename(ui->dumpfnLE->text());
    mdrLoader->setFirmwareFilename(ui->fwfnLE->text());
    //mdrLoader->setFlasherFilename(ui->flasherfnLE->text());
    //mdrLoader->setBaseAddress();
    mdrLoader->setMcuType((Mcu::Type)ui->mcuSelectCB->itemData(
                              ui->mcuSelectCB->currentIndex()).toInt());

    VasiliLoaderClient::WorkType work = VasiliLoaderClient::NoWork;
    if(ui->dumpCB->isChecked())    work |= VasiliLoaderClient::Dump;
    if(ui->eraseCB->isChecked())   work |= VasiliLoaderClient::Erase;
    if(ui->programCB->isChecked()) work |= VasiliLoaderClient::Program;
    if(ui->verifyCB->isChecked())  work |= VasiliLoaderClient::Verify;
    if(ui->runCB->isChecked())     work |= VasiliLoaderClient::Run;

    emit operate(work);
}

void MdrFwUart::loaderFinished()
{
    ui->startStopPB->setChecked(false);
    ui->startStopPB->setText("Start");
    ui->startStopPB->setEnabled(true);
    qDebug("work finished.\n-------------------------------------------------");
}

void MdrFwUart::on_dumpCB_clicked(bool checked)
{
    ui->dumpfnLE->setEnabled(checked);
    ui->dumpfnPB->setEnabled(checked);
}

void MdrFwUart::on_openFilePB_clicked()
{
    QString fn = QFileDialog::getOpenFileName(this,
                                              U("Файл прошивки"),
                                              ui->fwfnLE->text(),
                                              Hex::supportedFormats());
    ui->fwfnLE->setText(QDir::toNativeSeparators(fn));
}

void MdrFwUart::on_dumpfnPB_clicked()
{
    QString fn = QFileDialog::getSaveFileName(this,
                                              U("Файл дампа"),
                                              ui->dumpfnLE->text(),
                                              Hex::supportedFormats());
    ui->dumpfnLE->setText(QDir::toNativeSeparators(fn));
}

void MdrFwUart::on_flasherfnPB_clicked()
{
    QString fn = QFileDialog::getOpenFileName(this,
                                              U("Файл флешера"),
                                              ui->dumpfnLE->text(),
                                              Hex::supportedFormats());
    ui->flasherfnLE->setText(QDir::toNativeSeparators(fn));
}

void MdrFwUart::on_action_Qt_triggered()
{
    QMessageBox::aboutQt(this);
}

void MdrFwUart::on_about_triggered()
{
    QString text = U(
"Эта версия программы во многом аналогична программам от vasili (forum.milandr.ru), "
"однако имеются и определённые различия:\n"
"- пытается быть чуть более информативной\n"
"- проверяет наличие флешера в ОЗУ МК перед синхронизацией\n"
"- какие-то мелочи (если они важны и существенны - дайте знать)\n"
"Планируется поддержка всех или почти всех МК Миландр.\n"
"Планируется консольная версия "
"(точнее совмещение в одном файле консольной и графической версии).\n"
"Доработать загрузчик для возможности частичного стирания основной памяти, "
"работы с информационной памятью.\n"
"Предложения и замечания принимаются и приветствуются в личку на форуме "
"forum.milandr.ru пользователю prostoRoman или на prostoRoman@gmail.com\n"
"\nЛицензия, исходники позже. Информация здесь может быть уже не актуальна ;)\n"
"03.08.2017 Роман\n\n");
    text.append(U("Версия ПО: %1\n").arg(VERION));
    text.append(U("Дата сборки ПО: %1\n").arg(BUILD_DATE));
    QMessageBox mb(QMessageBox::Information,
                   U("О программе ").append(windowTitle()),
                   text, QMessageBox::Ok, this );
    mb.exec();
}

void MdrFwUart::logText(QString txt)
{
	qDebug() << "Text:" << txt;
}

void MdrFwUart::logStatus(QString st)
{
	qDebug() << "Status:" << st;
}

void MdrFwUart::updateComPortCB()
{
	QString current = ui->portNameCB->currentText();
	ui->portNameCB->clear();
	
	QList<QSerialPortInfo> list = QSerialPortInfo::availablePorts();
	std::sort(list.begin(), list.end(), [](QSerialPortInfo i1, QSerialPortInfo i2) -> bool {
		return i1.portName() < i2.portName();
	});
	foreach(auto i, list) {
		ui->portNameCB->addItem(i.portName().append(" = ")+i.description(),
		                        QVariant(i.systemLocation()));
	}
	ui->portNameCB->setCurrentIndex(ui->portNameCB->findText(current));
}

void MdrFwUart::writeSettings()
{
    QSettings settings;
    //qDebug()<<"sett file"<<settings.fileName();
    settings.beginGroup("MainWindow");
    settings.setValue("size", size());
    settings.setValue("pos", pos());
    if(fwfnParam.isEmpty()) {
        settings.setValue("fwfn", ui->fwfnLE->text());
    }
    settings.setValue("flasherfn", ui->flasherfnLE->text());
    settings.setValue("mcuSelect", ui->mcuSelectCB->itemData(
                          ui->mcuSelectCB->currentIndex()).toInt());
    settings.setValue("dump", ui->dumpCB->isChecked());
    settings.setValue("erase", ui->eraseCB->isChecked());
    settings.setValue("program", ui->programCB->isChecked());
    settings.setValue("verify", ui->verifyCB->isChecked());
    settings.setValue("run", ui->runCB->isChecked());
    settings.setValue("baudrate", ui->portBaudrateCB->currentText().toUInt());
    settings.setValue("portname", ui->portNameCB->itemData(
                                  ui->portNameCB->currentIndex()).toString());
    settings.endGroup();
}

void MdrFwUart::readSettings()
{
    QSettings settings;
    //qDebug()<<"sett file"<<settings.fileName();
    settings.beginGroup("MainWindow");
    resize(settings.value("size", QSize(720, 640)).toSize());
    move(settings.value("pos", QPoint(200, 200)).toPoint());
    if(fwfnParam.isEmpty()) {
        ui->fwfnLE->setText(settings.value("fwfn").toString());
    } else {
        ui->fwfnLE->setText(fwfnParam);
    }
    ui->flasherfnLE->setText(settings.value("flasherfn").toString());
    ui->mcuSelectCB->setCurrentIndex(
        ui->mcuSelectCB->findData(
            settings.value("mcuSelect", Mcu::UndefinedMcu).toString()));
    ui->dumpCB->  setChecked(settings.value("dump", false).toBool());
    ui->dumpfnLE->setEnabled(settings.value("dump", false).toBool());
    ui->dumpfnPB->setEnabled(settings.value("dump", false).toBool());
    ui->eraseCB->setChecked(settings.value("erase", true).toBool());
    ui->programCB->setChecked(settings.value("program", true).toBool());
    ui->verifyCB->setChecked(settings.value("verify", true).toBool());
    ui->runCB->setChecked(settings.value("run", true).toBool());
    //qDebug()<<"sett baud ="<<settings.value("baudrate", 115200).toUInt();
    //qDebug()<<"sett find ="<<ui->portBaudrateCB->findText(settings.value("baudrate", 115200).toString());
    ui->portBaudrateCB->setCurrentIndex(
        ui->portBaudrateCB->findText(
            settings.value("baudrate", 115200).toString()));
    ui->portNameCB->setCurrentIndex(
        ui->portNameCB->findData(
            settings.value("portname", "\\\\.\\COM1").toString()));
    settings.endGroup();
}




/*** https://evileg.com/ru/post/154/
 *   http://pavelk.ru/qt-nastraivaem-logirovanie-v-proekte-format-loga
 * sptintf, fprintf
 ***/
/*
 * для разделения стандартного вывода и вывода ошибок под виндой:
 * >MdrFwUart.exe --no-gui ... 1>out.txt 2>err.txt
 * */
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
//TODO сделать вывод зависимым от gui.
    QByteArray localMsg = msg.toLocal8Bit();
    const char *s = localMsg.constData();
    const char *filename = context.file ? context.file+13 : ""; // cut "../MdrFwUart/"
    const char *function = context.function ? context.function : "";
    switch (type) {
    case QtDebugMsg:
        fprintf(stderr, "Debug: %s (%s:%u, %s)\r\n", s, filename, context.line, function);
        break;
    case QtInfoMsg:
        fprintf(stderr, "Info: %s (%s:%u, %s)\r\n", s, filename, context.line, function);
        break;
    case QtWarningMsg:
        fprintf(stderr, "Warning: %s (%s:%u, %s)\r\n", s, filename, context.line, function);
        break;
    case QtCriticalMsg:
        fprintf(stderr, "Critical: %s (%s:%u, %s)\r\n", s, filename, context.line, function);
        break;
    case QtFatalMsg:
        fprintf(stderr, "Fatal: %s (%s:%u, %s)\r\n", s, filename, context.line, function);
        abort();
    }
    fflush(stderr);
}
#else
void myMessageHandler(QtMsgType type, const char *s) // s == msg
{
    switch (type) {
    case QtDebugMsg:
        fprintf(stderr, "Debug: %s\r\n", s);
        break;
    //case QtInfoMsg:
    case QtWarningMsg:
        fprintf(stderr, "Warning: %s\r\n", s);
        break;
    case QtCriticalMsg:
        fprintf(stderr, "Critical: %s\r\n", s);
        break;
    case QtFatalMsg:
        fprintf(stderr, "Fatal: %s\r\n", s);
        abort();
    }
    fflush(stderr);
}
#endif

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
     * 5. Проверить, что с ВЕ3 не будет той же проблемы, что и с ВЦ1 ???
     * */

#ifdef Q_OS_WIN
  QTextCodec::setCodecForLocale(QTextCodec::codecForName("IBM866"));
#endif

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
  qInstallMessageHandler(myMessageOutput);
#else
  qInstallMsgHandler(myMessageHandler);
#endif
    QScopedPointer<QCoreApplication> app(createApplication(argc, argv));
    
    qRegisterMetaType<VasiliLoaderClient::WorkType>("VasiliLoaderClient::WorkType");
    QCoreApplication::setOrganizationName("prostoRoman");
    QCoreApplication::setOrganizationDomain("prostoRoman.com");
    QCoreApplication::setApplicationName("MdrFwUart");
    QSettings::setDefaultFormat(QSettings::IniFormat);
    
    QApplication *a(qobject_cast<QApplication *>(app.data()));
    if (a) {
       // start GUI version...
        MdrFwUart w((QString(argv[1])));
        w.show();
        return app->exec();
    } else {
       // TODO start mdrfwucli from resource
    }
    return EXIT_FAILURE;
}



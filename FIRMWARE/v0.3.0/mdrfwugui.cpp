#include <stdio.h>
#include <stdlib.h>
#include <QApplication>
#include <QDebug>
#include <QDateTime>
#include <QDir>
#include <QFileDialog>
#include <QMessageBox>
#include <QSerialPortInfo>
#include <QSettings>
#include <QSettings>
#include <QTextCodec>

#include "mdrfwugui.h"
#include "ui_mdrfwuform.h"

#include "hex.h"
#include "mdrmcudescriptor.h"

#ifdef Q_OS_LINUX
#define CLINAME "mdrfwucli"
static const QTextCodec *codec = QTextCodec::codecForName("UTF-8");
#else //Q_OS_WIN
#  ifdef Q_OS_WIN64
#  define CLINAME "mdrfwucli64.exe"
#  else
#  define CLINAME "mdrfwucli.exe"
#  endif
static const QTextCodec *codec = QTextCodec::codecForName("IBM866");
#endif

#define U QString::fromUtf8

MdrFwUgui::MdrFwUgui(QString f, QWidget *parent) :
    QMainWindow(parent),
    fwfnParam(f),
    ui(new Ui::MdrFwUForm)
{
    ui->setupUi(this);
    setWindowTitle(U("MdrFwUgui v.%1.%2.%3")
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
    
#if (QT_VERSION >= QT_VERSION_CHECK(5, 2, 0))
    connect(&cli, &QProcess::readyReadStandardOutput, this, &MdrFwUgui::printSO);
    connect(&cli, &QProcess::readyReadStandardError,  this, &MdrFwUgui::printSE);
    connect(&cli, &QProcess::errorOccurred,           this, &MdrFwUgui::cliEO);
  //connect(&cli, &QProcess::finished,                this, &MainWindow::cliF);
    connect(&cli, &QProcess::stateChanged,            this, &MdrFwUgui::cliSC);
  //connect(&cli, &QProcess::started,                 this, &MainWindow::cliS);
#else
    connect(&cli, SIGNAL(readyReadStandardOutput()), this, SLOT(printSO()));
    connect(&cli, SIGNAL(readyReadStandardError()),  this, SLOT(printSE()));
    connect(&cli, SIGNAL(error/*Occurred*/(QProcess::ProcessError)),
            this, SLOT(cliEO(QProcess::ProcessError)));
  //connect(&cli, &QProcess::finished,                this, &MainWindow::cliF);
    connect(&cli, SIGNAL(stateChanged(QProcess::ProcessState)),
            this, SLOT(cliSC(QProcess::ProcessState)));
  //connect(&cli, &QProcess::started,                 this, &MainWindow::cliS);
#endif
    readSettings();
}

MdrFwUgui::~MdrFwUgui()
{
  QCloseEvent e;
  closeEvent(&e); // FIXME fix cli stopping correct.
}

void MdrFwUgui::printSO()
{
	QByteArray ba = cli.readAllStandardOutput();
	auto bal = ba.split('\n');
	foreach (ba, bal) {
		ba.replace("\r\n", "");
		ba.replace('\r', "");
		ba.replace('\n', "");
		if(ba.startsWith("Status: ")) {
			ba.remove(0, 8); // remove "Status: "
			if(ba.startsWith("stopButtonSetEnable")) {
				if(ba.contains("true") ) ui->startStopPB->setEnabled(true);
				if(ba.contains("false")) ui->startStopPB->setEnabled(false);
			}
			if(ba.startsWith("Work finished")) {
				loaderFinished();
				ba.append(QDateTime::currentDateTime().toString(" hh:mm:ss").toLatin1());
			}
			ui->sL->setText(codec->toUnicode(ba));
			logStatus(codec->toUnicode(ba));
		}
		else if(ba.startsWith("Text: ")) {
			ba.remove(0, 6); // remove "Text: "
			ui->t->appendPlainText(codec->toUnicode(ba));
			logText(codec->toUnicode(ba));
		}
		else if(ba.startsWith("DumpFNch: ")) {
			qDebug()<<codec->toUnicode(ba);
			ba.remove(0, 10); // remove "DumpFNch: "
			ui->dumpfnLE->setText(codec->toUnicode(ba));
			ui->t->appendHtml(U("<font color=\"blue\">Изменено имя дампа на %1</font>")
			                  .arg(codec->toUnicode(ba)));
		} else if(!ba.isEmpty()) {
			qDebug()<<codec->toUnicode(ba);
			ui->t->appendHtml(U("<font color=\"brown\">"
			                    "Неформализованное сообщение от CLI: %1</font>")
			                  .arg(codec->toUnicode(ba)));
		}
	}
}

void MdrFwUgui::printSE()
{
	QByteArray ba = cli.readAllStandardError();
	qDebug()<<codec->toUnicode(ba);
	ba.replace('\n', "<br>");
	ui->t->appendHtml(U("<font color=\"red\">%1</font>").arg(codec->toUnicode(ba)));
}

void MdrFwUgui::cliEO(QProcess::ProcessError error)
{
	QString s;
	switch (error) {
		case QProcess::FailedToStart: s = U("Failed To Start (file not found, resource error)"); break;
		case QProcess::Crashed: s = U("Crashed"); break;
		case QProcess::Timedout: s = U("Timedout"); break;
		case QProcess::ReadError: s = U("Read Error"); break;
		case QProcess::WriteError: s = U("Write Error"); break;
		case QProcess::UnknownError: s = U("Unknown Error"); break;
	}
	s.prepend(U("Server error is "));
	s.append(".");
	ui->sL->setText(s);
	logStatus(s);
	// NOTE loaderFinished();
}

/*void MainWindow::cliF(int exitCode, QProcess::ExitStatus exitStatus)
{
  ui->t->appendPlainText(U("cli process exit code %1, exit status %2.")
                         .arg(exitCode)
                         .arg(exitStatus)
                         );
}*/

void MdrFwUgui::cliSC(QProcess::ProcessState newState)
{
	QString s;
	switch (newState) {
		case QProcess::NotRunning: s = U("Not Running"); break;
		case QProcess::Starting:   s = U("Starting"); break;
		case QProcess::Running:    s = U("Running"); break;
	}
	s.prepend(U("Server state is "));
	s.append(".");
	if(newState == QProcess::NotRunning) {
		loaderFinished();
	} else {
		ui->sL->setText(s);
	}
	logStatus(s);
}

/*void MainWindow::cliS()
{
	ui->t->appendPlainText(U("cli process started."));
}*/

void MdrFwUgui::closeEvent(QCloseEvent *event)
{
	if (/* DISABLES CODE */ (true) /*userReallyWantsToQuit()*/ ) {
		loaderFinished();
		writeSettings();
		event->accept();
	} else {
		event->ignore();
	}
}

void MdrFwUgui::on_startStopPB_clicked()
{
    bool ok;
    QStringList args;
    args << "--gui"; // включить выфод нового формата.
    
    if(ui->startStopPB->isChecked() == false) {
      ui->t->appendHtml(U("<font color=\"red\"><b>"
                          "Принудительно остановлено пользователем."
                          "</font></b>"));
      cli.terminate();// с консольными приложениями не работает, см. help
      cli.kill();
      cli.waitForFinished(5000);
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

    args << U("--port=") + ui->portNameCB->itemData(
              ui->portNameCB->currentIndex()).toString();
    args << U("--speed=%1").arg(ui->portBaudrateCB->currentText().toUInt(&ok));
    args << U("--dump=") + ui->dumpfnLE->text();
    args << U("--fw=") + ui->fwfnLE->text();
//args << U("--flasher=") + ui->flasherfnLE->text();
//args << U("--baseAddress") +

    QString mcuType("--mcu=");
    mcuType.append(Mcu::shortName[ui->mcuSelectCB->itemData(
                ui->mcuSelectCB->currentIndex()).toInt()]);
    args << mcuType;

    QString work("-");
    if(ui->dumpCB->isChecked())    work.append("d");
    if(ui->eraseCB->isChecked())   work.append("e");
    if(ui->programCB->isChecked()) work.append("p");
    if(ui->verifyCB->isChecked())  work.append("v");
    if(ui->runCB->isChecked())     work.append("r");
    args << work;

    auto p = QCoreApplication::applicationDirPath();
    p = QDir::fromNativeSeparators(p);
    p = QDir::toNativeSeparators(p);
    p += QDir::separator() + U(CLINAME);
    if( !QFile::exists(p)) {
      QFile b(p);
      ui->t->appendHtml(U("<font color=\"red\">Could not found CLI.</font>"));
      if( !b.open(QIODevice::WriteOnly) ) {
        ui->t->appendHtml(U("<font color=\"red\">Could not create file %1</font>").arg(p));
      } else {
        QFile a( ":/" CLINAME );
        if(a.open(QIODevice::ReadOnly) == false) {
          ui->t->appendHtml(U("<font color=\"red\">Resource not found %1</font>").arg(a.fileName()));
          b.close();
          b.remove();
          return;
        }
        b.write(a.readAll());
        b.close();
#ifdef Q_OS_LINUX
        system(U("chmod +x %1").arg(p).toLatin1().constData());
#endif
      }
    }
    qDebug()<<"program"<<p<<"cli args"<<args;
    //cli.setProgram(p);
    //cli.setArguments(args);
    cli.setProcessChannelMode(QProcess::SeparateChannels);
    cli.start(p ,args);
}

void MdrFwUgui::loaderFinished()
{
	cli.terminate();// с консольными приложениями не работает, см. help
	cli.kill();
	cli.waitForFinished(5000);
	cli.kill();
	ui->startStopPB->setChecked(false);
	ui->startStopPB->setText("Start");
	ui->startStopPB->setEnabled(true);
	updateComPortCB();
}

void MdrFwUgui::on_dumpCB_clicked(bool checked)
{
	ui->dumpfnLE->setEnabled(checked);
	ui->dumpfnPB->setEnabled(checked);
}

void MdrFwUgui::on_openFilePB_clicked()
{
    QString fn = QFileDialog::getOpenFileName(this,
                                              U("Файл прошивки"),
                                              ui->fwfnLE->text(),
                                              Hex::supportedFormats());
    ui->fwfnLE->setText(QDir::toNativeSeparators(fn));
}

void MdrFwUgui::on_dumpfnPB_clicked()
{
    QString fn = QFileDialog::getSaveFileName(this,
                                              U("Файл дампа"),
                                              ui->dumpfnLE->text(),
                                              Hex::supportedFormats());
    ui->dumpfnLE->setText(QDir::toNativeSeparators(fn));
}

void MdrFwUgui::on_flasherfnPB_clicked()
{
    QString fn = QFileDialog::getOpenFileName(this,
                                              U("Файл флешера"),
                                              ui->dumpfnLE->text(),
                                              Hex::supportedFormats());
    ui->flasherfnLE->setText(QDir::toNativeSeparators(fn));
}

void MdrFwUgui::on_action_Qt_triggered()
{
    QMessageBox::aboutQt(this);
}

void MdrFwUgui::on_about_triggered()
{
    QString text = U(
    "Эта версия программы во многом аналогична программам от vasili (forum.milandr.ru), "
    "однако имеются и определённые различия:\n"
    "- разделена на консольную утилиту и графический интерфейс к ней\n"
    "- пытается быть чуть более информативной\n"
    "- проверяет наличие флешера в ОЗУ МК перед синхронизацией\n"
    "Планируется поддержка всех или почти всех МК Миландр.\n"
    "Планируется новый загрузчик, с возможностью частичного стирания основной памяти, "
                "поддрежки работы с информационной памятью.\n"
    "Предложения и замечания принимаются и приветствуются в личку на форуме "
    "forum.milandr.ru пользователю prostoRoman или на prostoRoman@gmail.com\n"
    "\nЛицензия, исходники позже. Информация здесь может быть уже не актуальна ;)\n"
    "18.11.2020 Роман\n\n");
    text.append(U("Версия ПО: %1\n").arg(VERION));
    text.append(U("Дата сборки ПО: %1\n").arg(BUILD_DATE));
    QMessageBox mb(QMessageBox::Information,
                   U("О программе ").append(windowTitle()),
                   text, QMessageBox::Ok, this );
    mb.exec();
}

void MdrFwUgui::logText(QString txt)
{
	qDebug() << "Text:" << txt;
}

void MdrFwUgui::logStatus(QString st)
{
	qDebug() << "Status:" << st;
}

void MdrFwUgui::updateComPortCB()
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

void MdrFwUgui::writeSettings()
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

void MdrFwUgui::readSettings()
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
    const char *filename = context.file ? context.file+13 : ""; // cut "../MdrFwUgui/"
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

int main(int argc, char *argv[])
{
/* TODO: что хочется добавить в проект: (проверить ВЕ3)
 * 2. работу с qesp
 * 3. с доработкой загрузчика:
 *    - частичное стирание флешки
 *    - работа с инфо областью
 * 5. Проверить, что с ВЕ3 не будет той же проблемы, что и с ВЦ1 ???
 * */
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    qInstallMessageHandler(myMessageOutput);
#else
    qInstallMsgHandler(myMessageHandler);
#endif
    QScopedPointer<QCoreApplication> app(new QApplication(argc, argv));
    
    QCoreApplication::setOrganizationName("prostoRoman");
    QCoreApplication::setOrganizationDomain("prostoRoman.com");
    QCoreApplication::setApplicationName("MdrFwUgui");
    QSettings::setDefaultFormat(QSettings::IniFormat);

    MdrFwUgui w(argc>1 ? (QString(argv[1])) : "" );
    w.show();
    return app->exec();
}

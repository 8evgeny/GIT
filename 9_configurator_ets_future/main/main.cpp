#include <QApplication>
#include "stdio.h"
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include "classes/app.h"
#include "main_window.h"
#include "include/typedefs.h"
#include "classes/base_model/items/stations_item/stations_item.h"
#include "dialogs/cryptodlg/test_cryptodlg.h"
#include "dialogs/communication/test_communication.h"
#include "dialogs/configdlg/test_configuredlg.h"
#include "dialogs/fmwaredlg/test_fmwaredlg.h"
#include "dialogs/msgitemsmodel/test_msgtablemodel.h"
#include "dialogs/debugdlg/test_debugviewdlg.h"
#include "dialogs/servicedlg/test_serviceviewdlg.h"
#include "classes/base_model/items/stations_item/test_stations_item.h"
#include "classes/base_model/items/station_item/test_station_item.h"

#if defined(__clang__)
	#pragma clang diagnostic push
	#pragma clang diagnostic warning "-Wconversion"
	#pragma clang diagnostic warning "-Wsign-conversion"
	#pragma clang diagnostic warning "-Wold-style-cast"
#elif defined(__GNUC__) || defined(__GNUG__)
	#pragma GCC diagnostic push
	#pragma GCC diagnostic warning "-Wconversion"
	#pragma GCC diagnostic warning "-Wsign-conversion"
	#pragma GCC diagnostic warning "-Wold-style-cast"
#elif defined(_MSC_VER)
#endif

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

void usage(void)
{
	std::cout << "Usage: ips-configurator.exe [options]\n"
  "	--help			Display this usage.\n"
  "	/?			Display this usage.\n"
  "	--test <sub options>	Display unit test programm\n"
 "Sub options with unit test only:\n"
 "	--silent		Silent output; only shows fatal errors, test failures and minimal status messages\n"
 "	--v2			Extended verbose output; shows each QCOMPARE() and QVERIFY(). (This option affects all output formats and implies -v1 for plain text output.)\n";
 //"	--vs			Shows all signals that get emitted and the slot invocations resulting from those signals. (This option affects all output formats.)\n";
} 

int main(int argc, char *argv[])
{
	QStringList* st;
	bool _istest = false;
    App a(argc, argv);
    addStyle();
    initPaths();
	if (argc > 1){
		for (int _i = 1; _i <= argc; _i++){
			if (QString::fromLocal8Bit(argv[_i]) == QString::fromLocal8Bit("--help")){ 
				usage();
				return 0;
			}
			if (QString::fromLocal8Bit(argv[_i]) == QString::fromLocal8Bit("/?")){ 
				usage();
				return 0;
			}
			if (QString::fromLocal8Bit(argv[_i]) == QString::fromLocal8Bit("--test")){ 
				std::cout << a.exeName().toLocal8Bit().constData() << '\n';
				_istest = true;
				st = new QStringList();
//				st->append(QString::fromLocal8Bit(argv[0]));
				st->append(a.exeName());
				st->append(QString::fromLocal8Bit("-o"));
				st->append(QString::fromLocal8Bit("testres.txt"));
				st->append(QString::fromLocal8Bit("-txt"));
				st->append(QString::fromLocal8Bit("-v1"));
			}
			if (QString::fromLocal8Bit(argv[_i]) == QString::fromLocal8Bit("--silent")){ 
				if (_istest)
					st->append(QString::fromLocal8Bit("-silent"));
			}
			if (QString::fromLocal8Bit(argv[_i]) == QString::fromLocal8Bit("--v2")){ 
				if (_istest)
					st->append(QString::fromLocal8Bit("-v2"));
			}
//			if (QString::fromLocal8Bit(argv[_i]) == QString::fromLocal8Bit("--vs")){ 
//				if (_istest)
//					st->append(QString::fromLocal8Bit("-vs"));
//			}
		}
		if (_istest){
			QFile _f(QString::fromLocal8Bit("testres.txt"));
			QTest::qExec(new Test_CryptoDialog, *st);
			if (_f.open(QIODevice::ReadOnly | QIODevice::Text)){
				QTextStream in(&_f);
				while (!in.atEnd()) {
					QString line = in.readLine();
					std::cout << line.toLocal8Bit().constData() << '\n';
				}
				_f.close();
			}
			QTest::qExec(new Test_Communication, *st);
			if (_f.open(QIODevice::ReadOnly | QIODevice::Text)){
				QTextStream in(&_f);
				while (!in.atEnd()) {
					QString line = in.readLine();
					std::cout << line.toLocal8Bit().constData() << '\n';
				}
				_f.close();
			}
			QTest::qExec(new Test_ConfigureDialog, *st);
			if (_f.open(QIODevice::ReadOnly | QIODevice::Text)){
				QTextStream in(&_f);
				while (!in.atEnd()) {
					QString line = in.readLine();
					std::cout << line.toLocal8Bit().constData() << '\n';
				}
				_f.close();
			}
			QTest::qExec(new Test_FmwareDialog, *st);
			if (_f.open(QIODevice::ReadOnly | QIODevice::Text)){
				QTextStream in(&_f);
				while (!in.atEnd()) {
					QString line = in.readLine();
					std::cout << line.toLocal8Bit().constData() << '\n';
				}
				_f.close();
			}
			QTest::qExec(new Test_MsgTableModel, *st);
			if (_f.open(QIODevice::ReadOnly | QIODevice::Text)){
				QTextStream in(&_f);
				while (!in.atEnd()) {
					QString line = in.readLine();
					std::cout << line.toLocal8Bit().constData() << '\n';
				}
				_f.close();
			}
			QTest::qExec(new Test_DebugViewDialog, *st);
			if (_f.open(QIODevice::ReadOnly | QIODevice::Text)){
				QTextStream in(&_f);
				while (!in.atEnd()) {
					QString line = in.readLine();
					std::cout << line.toLocal8Bit().constData() << '\n';
				}
				_f.close();
			}
			QTest::qExec(new Test_ServiceViewDialog, *st);
			if (_f.open(QIODevice::ReadOnly | QIODevice::Text)){
				QTextStream in(&_f);
				while (!in.atEnd()) {
					QString line = in.readLine();
					std::cout << line.toLocal8Bit().constData() << '\n';
				}
				_f.close();
			}
			QTest::qExec(new Test_StationsItem, *st);
			if (_f.open(QIODevice::ReadOnly | QIODevice::Text)) {
				QTextStream in(&_f);
				while (!in.atEnd()) {
					QString line = in.readLine();
					std::cout << line.toLocal8Bit().constData() << '\n';
				}
				_f.close();
			}
			QTest::qExec(new Test_StationItem, *st);
			if (_f.open(QIODevice::ReadOnly | QIODevice::Text)) {
				QTextStream in(&_f);
				while (!in.atEnd()) {
					QString line = in.readLine();
					std::cout << line.toLocal8Bit().constData() << '\n';
				}
				_f.close();
			}
			QFile::remove(QString::fromLocal8Bit("testres.txt"));
			return 0;
		}
	}
    MainWindow w;
    w.showFullScreen();
    return a.exec();
}
#if defined(__clang__)
	#pragma clang diagnostic pop
#elif defined(__GNUC__) || defined(__GNUG__)
	#pragma GCC diagnostic pop
#elif defined(_MSC_VER)
#endif

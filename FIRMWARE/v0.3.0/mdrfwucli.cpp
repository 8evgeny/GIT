/*
 * 
 * */

#include "vasililoader.h"

#include <stdio.h>
#include <stdlib.h>
#include <QCoreApplication>
#include <QDebug>
#include <QSettings>
#include <QSerialPortInfo>
#include <QtGlobal>
#include <QTextCodec>

static VasiliLoaderClient::WorkType work;
static Mcu::Type mcuType;
static QString portName, firmwareName, dumpFileName, flasherFileName;
static int speed;
static bool gui = false; // NOTE сменился контекст: эта опция означает работу из под ГУЯ и меняет вывод программы.
static bool help = false;
static bool printVersion = false;
static bool printDebug = false;
static uint32_t baseAddress = Hex::FORBIDDEN_ADDRESS;
static int retcode;
static VasiliLoaderClient vLdr;

#if (QT_VERSION < QT_VERSION_CHECK(5, 2, 0))
#include "signalhelper.h"

SignalHelper::SignalHelper() {}

void SignalHelper::statusChanged(QString st) {
  if(gui) {
    fprintf(stdout, "Status: %s\r\n", st.toLocal8Bit().constData());
  } else {
    fprintf(stdout, ".");
  }
  fflush(stdout);
}

void SignalHelper::textChanged(QString txt) {
  fprintf(stdout, gui?"Text: %s\r\n":"%s\r\n", txt.toLocal8Bit().constData());
  fflush(stdout);
}

void SignalHelper::workFinished() {
  if(gui){
    fprintf(stdout, "Status: Work finished.\r\n");
  } else {
    fprintf(stdout, "work finished. -------------------------------------\r\n");
  }
  fflush(stdout);
}

void SignalHelper::stopButtonSetEnable(bool en) {
  if(gui){
    fprintf(stdout, "Status: stopButtonSetEnable %s.\r\n", en?"true":"flase");
    fflush(stdout);
  }
}

void SignalHelper::dumpFilenameChanged(QString dumpfn) {
  if(gui){
    fprintf(stdout, "DumpFNch: %s\r\n", dumpfn.toLocal8Bit().constData());
    fflush(stdout);
  }
}

static SignalHelper signalHelper;
#endif


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
    const char *filename = context.file ? context.file+13 : ""; // cut "../mdrfwucli/"
    const char *function = context.function ? context.function : "";
    switch (type) {
    case QtDebugMsg:
        if(printDebug) fprintf(stderr, "Debug: %s (%s:%u, %s)\r\n", s, filename, context.line, function);
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
        if(printDebug) fprintf(stderr, "Debug: %s\r\n", s);
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

bool parseArgs(int &argc, char *argv[])
{
  work = VasiliLoaderClient::NoWork;
  mcuType = Mcu::UndefinedMcu;
  portName.clear();
  firmwareName.clear();
  dumpFileName.clear();
  speed = 9600;

  for(int i=1; i<argc; ++i) {
    //printf("%d %s\n", i, argv[i]);
    char *c = argv[i];
    if(*c != '-') {
      printf("parseArgs: argument %d isn't start of '-'. Stop.\n", i);
      return false;
    }
    ++c;
    if(*c != '-') { // формат коротких аргументов
      while(*c != 0) {
        switch (*c) {
          case 'd': work |= VasiliLoaderClient::Dump; break;
          case 'e': work |= VasiliLoaderClient::Erase; break;
          case 'p': work |= VasiliLoaderClient::Program; break;
          case 'v': work |= VasiliLoaderClient::Verify; break;
          case 'r': work |= VasiliLoaderClient::Run; break;
          default:
            printf("parseArgs: short argument '%c' not supported. Stop.\n", *c);
            return false;
        }
        ++c;
      }
    } else { // длинный формат аргументов
      ++c; // сдвинули на третий символ - начало команды
      if(strcmp(c, "no-gui") == 0) continue;
      if(strcmp(c, "gui") == 0) {
        gui = true;
        continue;
      }
      if(strcmp(c, "help") == 0) {
        help = true;
        continue;
      }
      if(strcmp(c, "version") == 0) {
        printVersion = true;
        continue;
      }
      if(strcmp(c, "debug") == 0) {
        printDebug = true;
        continue;
      }
      if(memcmp(c, "speed=", 6) == 0) {
        c += 6;
        speed = atoi(c);
        if(speed <= 0 || speed > 1e6) {
          printf("parseArgs: speed '%d' not supported. Stop.\n", speed);
          return false;
        }
        continue;
      }
      if(memcmp(c, "mcu=", 4) == 0) {
        c += 4;
        if(strcmp(c, Mcu::shortName[Mcu::MDR1986BE9X]) == 0) { mcuType = Mcu::MDR1986BE9X; } else
        if(strcmp(c, Mcu::shortName[Mcu::MDR1901VC1]) == 0) { mcuType = Mcu::MDR1901VC1; } else
        if(strcmp(c, Mcu::shortName[Mcu::MDR1986BE1]) == 0) { mcuType = Mcu::MDR1986BE1; } else
        if(strcmp(c, Mcu::shortName[Mcu::MDR1986BE3]) == 0) { mcuType = Mcu::MDR1986BE3; } else
        if(strcmp(c, Mcu::shortName[Mcu::MDR1986BE4]) == 0) { mcuType = Mcu::MDR1986BE4; } else
        if(strcmp(c, Mcu::shortName[Mcu::UndefinedMcu]) == 0) { mcuType = Mcu::UndefinedMcu; }
        else {
          printf("parseArgs: unsupported MCU type '%s'. Stop.\n", c);
          return false;
        }
        continue;
      }
      if(memcmp(c, "port=", 5) == 0) {
        c += 5;
        portName.append(c);
        continue;
      }
      if(memcmp(c, "fw=", 3) == 0) {
        c += 3;
        firmwareName.append(c);
        continue;
      }
      if(memcmp(c, "dump=", 5) == 0) {
        c += 5;
        dumpFileName.append(c);
        continue;
      }
      if(memcmp(c, "flasher=", 8) == 0) {
        c += 8;
        flasherFileName.append(c);
        continue;
      }
      if(memcmp(c, "baseAddress=", 12) == 0) {
        c += 12;
        bool ok = false;
        baseAddress = QString(c).toUInt(&ok, 16);
        if(!ok) {
          printf("parseArgs: incorrect baseAddress. Stop.\n");
          baseAddress = Hex::FORBIDDEN_ADDRESS;
          return false;
        }
        continue;
      }
      // во всех условиях до этого должен быть continue, а это - д.б. последним!
      printf("parseArgs: unknown argument '--%s'. Stop.\n", c);
      return false;
    }
  }
  return true;
}

void printHelp()
{
  printf("\n"
  "mdrfwucli version %d.%d.%d  - %s - %s - Qt %s.\n",
         VER_MAJOR, VER_MINOR, VER_PATCH,  VERION, BUILD_DATE, QT_VERSION_STR);
  printf("\n"
  "Using format:\n"
  "mdrfwucli --help\n"
  "mdrfwucli --version\n"
  "mdrfwucli (--port=C)  (--mcu=M) ([-depvr]) (--fw=F) (--dump=D) "
         "(--speed=N) (--gui) (--debug) (--flasher=F) (--baseAddress=H)\n"
  "where:\n"
  "  C - standart communication port system name, for exapmle:\n"
  "        on MS Windows: COM1, \\\\.\\COM9, COM158...\n"
  "        on Linus: tty1, ttyUSB0, /dev/tty42...\n"
  "        By default - the only presetn port in the system.\n"
  "  D - dump file; supported formats .hex, .bin.\n"
  "  F - firmware file; supported formats .elf, .axf, .hex, .bin.\n"
  "      For .bin format base address set to MCU FLASH begin address.\n"
  "  H - in hex set custom base address for .bin.\n"
  "  N - one of standart communication port speed, by default 9600 bit/s.\n"
  "  M - one of supported Milandr MCU type:\n"
  "        ve9x - 1986BE91,92,93,94, MDR32F9Q2I series\n"
  "        vc1  - 1901VC1, MDR32FG16S1\n"
  "        ve1  - 1986BE1, MDR32F1\n"
  "        ve3  - 1986BE3\n"
  "        ve4  - 1986BE4\n"
  "        auto - for autodetection MCU type.\n"
  "\n"
  " -d - Dump MCU main flash in file D.\n"
  " -e - Erase MCU main flash.\n"
  " -p - Programm MCU main flash from file F.\n"
  " -v - Verify MCU main flash from file F.\n"
  " -r - Run programm from MCU main flash.\n");
}

/* раскрашивание текста UNIX
 https://habr.com/ru/post/94647/
 man console_codes
  атрибуты
  0 	нормальный режим
  1 	жирный
  4 	подчеркнутый
  5 	мигающий
  7 	инвертированные цвета
  8 	невидимый
  цвет текста
  30 	черный
  31 	красный
  32 	зеленый
  33 	желтый
  34 	синий
  35 	пурпурный
  36 	голубой
  37 	белый
  цвет фона
  40 	черный
  41 	красный
  42 	зеленый
  43 	желтый
  44 	синий
  45 	пурпурный
  46 	голубой
  47 	белый
*/

int main(int argc, char *argv[])
{
#ifdef Q_OS_WIN
  QTextCodec::setCodecForLocale(QTextCodec::codecForName("IBM866"));
#endif

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
  qInstallMessageHandler(myMessageOutput);
#else
  qInstallMsgHandler(myMessageHandler);
#endif

  // Warning: QObject::startTimer: Timers can only be used with threads started with QThread (:0, )
  QCoreApplication app(argc, argv);

  qRegisterMetaType<VasiliLoaderClient::WorkType>("MdrLoader::WorkType");
  QCoreApplication::setOrganizationName("prostoRoman");
  QCoreApplication::setOrganizationDomain("prostoRoman.com");
  QCoreApplication::setApplicationName("mdrfwucli");
  QSettings::setDefaultFormat(QSettings::IniFormat);

  if(parseArgs(argc, argv) == false) {
    printf("main: Error at arguments parsed!\n");
    printHelp();
    return -2;
  }

  if(printVersion) {
    printf("%d.%d.%d - %s - %s\n",
           VER_MAJOR, VER_MINOR, VER_PATCH,  VERION, BUILD_DATE);
    return EXIT_SUCCESS;
  }
  if(help) {
    printHelp();
    return EXIT_SUCCESS;
  }
  if(printDebug) {
    printf("printDebug enabled.\n");
  }

  if(mcuType == Mcu::UndefinedMcu) {
#ifdef Q_OS_WIN
    //system("color 0E"); не даёт нужного результата - работать через SetConsoleTextAttribute()
    printf("main: MCU type been autodetected.\n");
#else
    printf("\x1b[33m"
           "main: MCU type been autodetected.\n"
           "\x1b[0m");
#endif
  }

  if(portName.isEmpty()) {
#ifdef Q_OS_WIN
    //system("color 0A");
    printf("main: communication port name is not defined.\n");
#else
    printf("\x1b[31m"
           "main: communication port name is not defined.\n"
           "\x1b[0m");
#endif
		auto ports = QSerialPortInfo::availablePorts();
		if(ports.count() == 1) {
			portName = ports.first().portName();
			printf("main: autoselect port is %s.\n", portName.toLatin1().constData());
		} else if (ports.count() == 0) {
			printf("main: not found serial communication port.\nStop.\n");
			printHelp();
			return -EXIT_FAILURE;
		} else {
			printf("main: Available ports:\n");
			foreach (auto port, ports) {
				printf("  - %-10s - %s\n",
				       port.portName().toLatin1().constData(),
				       port.description().toLatin1().constData());
			}
			printf("----------------\n");
			return -EXIT_FAILURE;
		}
  }

#if (QT_VERSION >= QT_VERSION_CHECK(5, 2, 0))
  
  QObject::connect(&vLdr, &VasiliLoaderClient::statusChanged, [](QString st) {
    if(gui) {
      fprintf(stdout, "Status: %s\r\n", st.toLocal8Bit().constData());
    } else {
      fprintf(stdout, ".");
    }
    fflush(stdout);
  });

  QObject::connect(&vLdr, &VasiliLoaderClient::textChanged, [] (QString txt) {
    fprintf(stdout, gui?"Text: %s\r\n":"%s\r\n", txt.toLocal8Bit().constData());
    fflush(stdout);
  });

  QObject::connect(&vLdr, &VasiliLoaderClient::workFinished, [](){
    if(gui){
      fprintf(stdout, "Status: Work finished.\r\n");
    } else {
      fprintf(stdout, "work finished. -------------------------------------\r\n");
    }
    fflush(stdout);
  });

  QObject::connect(&vLdr, &VasiliLoaderClient::stopButtonSetEnable, [](bool en){
    if(gui){
      fprintf(stdout, "Status: stopButtonSetEnable %s.\r\n", en?"true":"flase");
      fflush(stdout);
    }
  });

  QObject::connect(&vLdr, &VasiliLoaderClient::dumpFilenameChanged, [](QString dumpfn){
    if(gui){
      fprintf(stdout, "DumpFNch: %s\r\n", dumpfn.toLocal8Bit().constData());
      fflush(stdout);
    }
  });
#else
  signalHelper.gui = gui;
  
  QObject::connect(&vLdr,  SIGNAL(statusChanged(QString)),
                   &signalHelper, SLOT(statusChanged(QString)));
  
  QObject::connect(&vLdr,  SIGNAL(textChanged(QString)),
                   &signalHelper, SLOT(textChanged(QString)));
  
  QObject::connect(&vLdr,  SIGNAL(workFinished()),
                   &signalHelper, SLOT(workFinished()));

  QObject::connect(&vLdr,  SIGNAL(stopButtonSetEnable(bool)),
                   &signalHelper, SLOT(stopButtonSetEnable(bool)));
  
  QObject::connect(&vLdr,  SIGNAL(dumpFilenameChanged(QString)),
                   &signalHelper, SLOT(dumpFilenameChanged(QString)));
#endif
  
  vLdr.setPortName(portName);
  vLdr.setBaudRate(static_cast<QSerialPort::BaudRate>(speed));
  vLdr.setDumpFilename(dumpFileName);
  vLdr.setFirmwareFilename(firmwareName);
  vLdr.setFlasherFilename(flasherFileName);
  vLdr.setBaseAddress(baseAddress);
  vLdr.setMcuType(mcuType);

  retcode = vLdr.doWorkNew(work) ? EXIT_SUCCESS : EXIT_FAILURE;

  return retcode;
}


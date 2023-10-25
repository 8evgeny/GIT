#include "main.h"
#include <boost/program_options.hpp>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
namespace po = boost::program_options;
#if 0

 #endif

map<string, string> readConfig(const char* conf_file) {
    map<string, string> config{};
    cout<<"reading config.ini file\n"<<endl;
    po::variables_map vm;
    po::options_description patches("patchesToDirectories");
    patches.add_options()
          ("patchesToDirectories.niokrPoOboznacheniyam", po::value<string>(), "")
          ("patchesToDirectories.niokrFoldersToSoftLinks", po::value<string>(), "");
  po::options_description desc("Allowed options");
  desc.add(patches);
  try {
    po::parsed_options parsed = po::parse_config_file<char>(conf_file, desc, true);  //флаг true разрешает незарегистрированные опции !
    po::store(parsed, vm);
  } catch (const po::reading_file& e) {
    std::cout << "Error: " << e.what() << std::endl;
  }
  po::notify(vm);
  config["niokrPoOboznacheniyam"] = vm["patchesToDirectories.niokrPoOboznacheniyam"].as<string>();
  config["niokrFoldersToSoftLinks"] = vm["patchesToDirectories.niokrFoldersToSoftLinks"].as<string>();
  return config;
}

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
    &app, [url](QObject * obj, const QUrl & objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    QQmlContext *context = engine.rootContext();
    engine.load(url);



    auto config = readConfig("../config.ini");
    cout << "niokrPoOboznacheniyam: \t" << config["niokrPoOboznacheniyam"] << endl;
    cout << "niokrFoldersToSoftLinks: \t" << config["niokrFoldersToSoftLinks"] << endl<<endl;
//QML перечень директорий



return app.exec();
}

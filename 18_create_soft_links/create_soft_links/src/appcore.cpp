#include "main.h"
#include "appcore.h"
#include <utility>
#include <QDir>
#include <QFile>
#include <QModelIndex>
#include <QThread>
#include <QTimer>
#include <qdatastream.h>

AppCore::AppCore(QObject *parent) : QObject(parent)
{

}

void AppCore::needCloseMainWindow()
{
    emit closeMainWindow();
}

void AppCore::exitActionFromMenu(){
    emit exitFromMenu();
}

void AppCore::readConfig(const char* conf_file) {
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
  cout << "niokrPoOboznacheniyam: \t" << config["niokrPoOboznacheniyam"] << endl;
  cout << "niokrFoldersToSoftLinks: \t" << config["niokrFoldersToSoftLinks"] << endl<<endl;
}

#include "main.h"
#include <algorithm>
#if 0
https://htmlbook.ru/content/svoystva-ssylok
docker run -it --rm -d -p 8081:80 --name web -v ~/SOFT/Github/GIT/19_update_WEB_ARHIV/CONTENT:/usr/share/nginx/html nginx
                                    или
docker run -it --rm -d -p 8080:80 --name web2 -v ~/SOFT/Github/GIT/16_WEB_ARHIV/CONTENT:/usr/local/apache2/htdocs/ httpd:2.4
docker build -t  createwebcontent ~/SOFT/Github/GIT/16_WEB_ARHIV
Один раз дать права на запись других
sudo chmod -R ugo+rwx ~/SOFT/Github/GIT/16_WEB_ARHIV/CONTENT/content_for_web

После отработки контейнера, чтобы пересобрать - удалить под root файлы из content_for_web  или
sudo chown -R evg:evg ~/SOFT/Github/GIT/16_WEB_ARHIV/CONTENT/content_for_web && chmod -R ugo+rwx ~/SOFT/Github/GIT/16_WEB_ARHIV/CONTENT/content_for_web

docker run --name=createwebcontent --rm -v ~/SOFT/Github/GIT/16_WEB_ARHIV/CONTENT:/home/evg/CONTENT -v ~/SOFT/Github/GIT/16_WEB_ARHIV/Ниокр-Актуальные_документы:/home/evg/Ниокр-Актуальные_документы -v ~/SOFT/Github/GIT/16_WEB_ARHIV/Ниокр-Документы_по_обозначениям:/home/evg/Ниокр-Документы_по_обозначениям -v ~/SOFT/Github/GIT/16_WEB_ARHIV/Ниокр-Извещения_об_изменении:/home/evg/Ниокр-Извещения_об_изменении -v ~/SOFT/Github/GIT/16_WEB_ARHIV/Ниокр-Неактуальные_документы:/home/evg/Ниокр-Неактуальные_документы -v /etc/timezone:/etc/timezone:ro -v /etc/localtime:/etc/localtime:ro createwebcontent
docker system prune
cd ~/SOFT/Github/GIT/16_WEB_ARHIV && docker save createwebcontent > createwebcontent.tar
cd ~/SOFT/Github/GIT/16_WEB_ARHIV && docker load -i createwebcontent.tar
#endif
string WEB_content{"../../CONTENT/content_for_web/"};
uint numContent{0};

int main(int argc, char *argv[])
{
    auto config = readConfig("../config.ini");
//    createMainWebContent(config);

//Тут формирую дополнительный контент (по изделиям)
//Сканирую директорию Ниокр-Папки_по_изделиям (Собираю поддиректории)
    vector<string> vectorFolders{};
    auto iteratorFolders = recursive_directory_iterator{ config["niokrFoldersByDevices"], directory_options::skip_permission_denied };
    for(const auto& entry : iteratorFolders) {
      try {
        if(!entry.is_directory())
          continue;
        string patch = entry.path().string();
        vectorFolders.push_back(patch);
      } catch(const exception& e) {
        cerr << "Error reading " << entry.path().string() << ": " << e.what() << endl;
      }
    }
//Сортируем вектор
    sort(vectorFolders.begin(), vectorFolders.end());

    cout<< "Директории с софтлинками:\n";
//Список директорий сохраняем в файле foldersList
    QFile ffold((config["niokrFoldersByDevices"]  + "/foldersList").c_str());
    ffold.open(QIODevice::WriteOnly);
    QByteArray ba;
        for (auto i:vectorFolders) {


        ba.append(QString::fromStdString(i));
        ba.append('\n');
        cout<<i<<endl;
    }
    ffold.write(ba);
    ffold.close();










//Запускаем web-server
    string webStart = "docker run -it --rm -d -p 8081:80 --name web -v ~/SOFT/Github/GIT/19_update_WEB_ARHIV/CONTENT:/usr/share/nginx/html nginx 2>/dev/null";
    system(webStart.c_str());
}

map<string, string> readConfig(const char* conf_file) {
    map<string, string> config{};
    cout<<"Читаем config.ini "<<endl;
    po::variables_map vm;
    po::options_description patches("patchesToDirectories");
    patches.add_options()
          ("patchesToDirectories.niokrActualDocs", po::value<string>(), "")
          ("patchesToDirectories.niokrPoOboznacheniyam", po::value<string>(), "")
          ("patchesToDirectories.niokrIzvesheniya", po::value<string>(), "")
          ("patchesToDirectories.niokrOldDocs", po::value<string>(), "")
          ("patchesToDirectories.niokrFoldersByDevices", po::value<string>(), "");
  po::options_description desc("Allowed options");
  desc.add(patches);
  try {
    po::parsed_options parsed = po::parse_config_file<char>(conf_file, desc, true);  //флаг true разрешает незарегистрированные опции !
    po::store(parsed, vm);
  } catch (const po::reading_file& e) {
    std::cout << "Error: " << e.what() << std::endl;
  }
  po::notify(vm);
  config["niokrActualDocs"] = vm["patchesToDirectories.niokrActualDocs"].as<string>();
  config["niokrPoOboznacheniyam"] = vm["patchesToDirectories.niokrPoOboznacheniyam"].as<string>();
  config["niokrIzvesheniya"] = vm["patchesToDirectories.niokrIzvesheniya"].as<string>();
  config["niokrOldDocs"] = vm["patchesToDirectories.niokrOldDocs"].as<string>();
  config["niokrFoldersByDevices"] = vm["patchesToDirectories.niokrFoldersByDevices"].as<string>();
  cout << "niokrActualDocs:       \t" << config["niokrActualDocs"] << endl;
  cout << "niokrPoOboznacheniyam: \t" << config["niokrPoOboznacheniyam"] << endl;
  cout << "niokrIzvesheniya:      \t" << config["niokrIzvesheniya"] << endl;
  cout << "niokrOldDocs:         \t" << config["niokrOldDocs"] << endl;
  cout << "niokrFoldersByDevices:  \t" << config["niokrFoldersByDevices"] << endl << endl;

  return config;
}

void createMainWebContent(map<string, string> &config){

    path archiv_path_zip{config["niokrActualDocs"]};

    string pathToExtractDirectory = "EXTRACT"; //в папке build
    const path archiv_path_extracted{pathToExtractDirectory};
//Разархивирование исходного контента
//Очищаем директорию для извлечения
    string eraseDir = " rm -rf ";
    eraseDir.append(archiv_path_extracted);
    eraseDir.append("/");
    system(eraseDir.c_str());

//Очищаем директорию web контента
    string eraseWebDir = " rm -rf ";
    eraseWebDir.append(WEB_content);
    system(eraseWebDir.c_str());
    string createWebContentDir = "mkdir ";
    createWebContentDir.append(WEB_content);
    system(createWebContentDir.c_str());

    vector<string> vectorZipFilesPath;
    vector<string> vectorZipFilesName;
    const unordered_set<string> zip_extensions{ ".zip" };
    auto iteratorZip = recursive_directory_iterator{ archiv_path_zip, directory_options::skip_permission_denied };
    for(const auto& entry : iteratorZip) {
      try {
        if(!entry.is_regular_file())
          continue;
        const auto& extension = entry.path().extension().string();
        const auto is_zip = zip_extensions.find(extension) != zip_extensions.end();
        if(!is_zip)
          continue;
        string patch = entry.path().string();
//        auto posBlank = patch.find(" "); //Борьба с пробелом (Пути файлов уже не содержат пробелов)
//        patch.insert(posBlank,"\\");
        string name = entry.path().filename();
        vectorZipFilesPath.push_back(patch);
        name.pop_back();//убираем .zip
        name.pop_back();
        name.pop_back();
        name.pop_back();
        vectorZipFilesName.push_back(name);
      } catch(const exception& e) {
        cerr << "Error reading " << entry.path().string() << ": " << e.what() << endl;
      }
    }

    if (vectorZipFilesPath.size()>0)
    {
        for (auto i = 0; i < vectorZipFilesPath.size();++i ){
            ++numContent;
            cout << endl<< numContent << ": "<< vectorZipFilesPath[i] << endl;
            extractZip(vectorZipFilesPath[i], vectorZipFilesName[i], pathToExtractDirectory); //распаковка одного zip файла
        }
    }
    cout<< endl<< "Всего разархивировано zip файлов: " << numContent<< endl;

//Разбор разархивированной директории
    vector<string> vectorJsonFilesPath;
    vector<string> errorJsonPatch;
    const unordered_set<string> json_extensions{ ".json" };
    cout << endl <<"Patch for json search: "<<archiv_path_extracted << endl;
    auto iteratorJson = recursive_directory_iterator{ archiv_path_extracted, directory_options::skip_permission_denied };
    for(const auto& entry : iteratorJson) {
      try {
        if(!entry.is_regular_file())
          continue;
        const auto& extension = entry.path().extension().string();
        const auto is_json = json_extensions.find(extension) != json_extensions.end();
        if(!is_json)
          continue;
        string patch = entry.path().string();
        vectorJsonFilesPath.push_back(patch);
      } catch(const exception& e) {
        cerr << "Error reading " << entry.path().string() << ": " << e.what() << endl;
      }
    }
    int errorParsingJson = 0;
    if (vectorJsonFilesPath.size()>0)
    {
        int num = 0;
        for (auto & patchJsonFile : vectorJsonFilesPath){
            ++num;
            cout << endl<< num << ": "<< patchJsonFile << endl;
            if(!parseJSON(patchJsonFile, archiv_path_zip)) //парсинг одного файла JSON
            {
                cout << "ОШИБКА JSON ФАЙЛА" << endl;
                ++errorParsingJson;
                errorJsonPatch.push_back(patchJsonFile);
            }
        }
    }
    cout<< endl<< "Всего разобрано json файлов: " << vectorJsonFilesPath.size()
       << endl << "Ошибок разбора: " << errorParsingJson<< endl;
    for (auto & patchJsonError : errorJsonPatch)
        cout <<  patchJsonError << endl;
}

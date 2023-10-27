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
//Вначале удаляю папку Ниокр-Папки_по_изделиям из web контента
    string cmdDel = "rm -rf ";
    cmdDel.append(WEB_content);
    string newNameRm = replaceAll(config["niokrFoldersByDevices"], " ", "\\ ");
    cmdDel.append(nameFromPath(newNameRm));
    system(cmdDel.c_str());
//Копирую папку Ниокр-Папки_по_изделиям в папку CONTENT/content_for_web (не линки а уже реальные файлы)
    string cmdCopy = "cp -r -L "; //-L, --dereference - копировать не символические ссылки, а то, на что они указывают

    string newNameCp = replaceAll(config["niokrFoldersByDevices"], " ", "\\ ");
    cmdCopy.append(newNameCp).append(" ").append(WEB_content);
    system(cmdCopy.c_str());

//Сканирую директорию Ниокр-Папки_по_изделиям (Собираю поддиректории)
    vector<string> vectorPathFolders{};
    vector<string> vectorNameFolders{};

    string foldersDevesesForWeb = WEB_content;
    foldersDevesesForWeb.append(nameFromPath(config["niokrFoldersByDevices"]));
    auto iteratorFolders = recursive_directory_iterator{foldersDevesesForWeb, directory_options::skip_permission_denied };
    for(const auto& entry : iteratorFolders) {
      try {
        if(!entry.is_directory())
          continue;
        string patch = entry.path().string();
        vectorPathFolders.push_back(patch);
      } catch(const exception& e) {
        cerr << "Error reading " << entry.path().string() << ": " << e.what() << endl;
      }
    }
//Сортируем вектор
    sort(vectorPathFolders.begin(), vectorPathFolders.end());
//из пути получаем имя
    for (auto & i:vectorPathFolders){
        vectorNameFolders.push_back(nameFromPath(i));
    }

//Список путей директорий сохраняем в файле pathFoldersList
    string pathF1 = WEB_content;
    pathF1.append("pathFoldersList");
    QFile f1(pathF1.c_str());
    f1.open(QIODevice::WriteOnly);
    QByteArray ba1;
    cout<< "Пути к директориям с софтлинками:\n";
        for (auto & i:vectorPathFolders) {
        ba1.append(QString::fromStdString(i));
        ba1.append('\n');
        cout<<i<<endl;
    }
    f1.write(ba1);
    f1.close();
//Имена изделий сохраняем в файле nameFoldersList
        string pathF2 = WEB_content;
        pathF2.append("nameFoldersList");
        QFile f2(pathF2.c_str());
        f2.open(QIODevice::WriteOnly);
        QByteArray ba2;
        cout<< "Имена папок с софтлинками:\n";
            for (auto & i:vectorNameFolders) {
            ba2.append(QString::fromStdString(i));
            ba2.append('\n');
            cout<<i<<endl;
        }
        f2.write(ba2);
        f2.close();
//Далее работа JS


//Запускаем web-server
    string webStart = "docker run -it --rm -d -p 8081:80 --name web -v ~/SOFT/Github/GIT/19_update_WEB_ARHIV/CONTENT:/usr/share/nginx/html nginx 2>/dev/null";
    system(webStart.c_str());
}

string nameFromPath(string path){
    auto pos = path.find_last_of('/');
    return path.substr(pos+1);
}

std::string replaceAll( std::string const& name, std::string const& before, std::string const& after ){
    std::string retval;
    std::string::const_iterator end     = name.end();
    std::string::const_iterator current = name.begin();
    std::string::const_iterator next    =
            std::search( current, end, before.begin(), before.end() );
    while ( next != end ) {
        retval.append( current, next );
        retval.append( after );
        current = next + before.size();
        next = std::search( current, end, before.begin(), before.end() );
    }
    retval.append( current, next );
    return retval;
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

#include "main.h"
#if 0

docker run -it --rm -d -p 8080:80 --name web -v ~/SOFT/Github/GIT/16_WEB_ARHIV/CONTENT:/usr/share/nginx/html nginx
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
string WEB_content{"/home/evg/SOFT/Github/GIT/16_WEB_ARHIV/CONTENT/content_for_web/"};
uint numContent{0};

int main(int argc, char *argv[])
{
    cout <<  "start docker servises" << endl;
    string dockerStart = "docker-compose up -d";
    system(dockerStart.c_str());

//    path archiv_path_zip{"/home/evg/SOFT/Github/GIT/16_WEB_ARHIV/Ниокр-Актуальные_документы"};
//    path archiv_path_zip{"/home/evg/SOFT/Github/GIT/16_WEB_ARHIV/_ERRORS"};
//    path archiv_path_zip{"/home/evg/SOFT/Github/GIT/16_WEB_ARHIV/_BAD"};
    path archiv_path_zip{"/home/evg/SOFT/Github/GIT/16_WEB_ARHIV/_TEST"};
    if (argc != 2){
//        cout << "Передается в качестве параметра путь: "<< archiv_path_zip << endl;
    }
    else{
        archiv_path_zip= argv[1];
    }

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
    cout << "Patch for zip search: "<<archiv_path_zip << endl;
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
            cout <<"Extract "<< numContent << ": "<< nameFromPath(vectorZipFilesPath[i]) << endl;
            extractZip(vectorZipFilesPath[i], vectorZipFilesName[i], pathToExtractDirectory); //распаковка одного zip файла
        }
    }
    cout<< "Всего разархивировано zip файлов: " << numContent<< endl;
    cout<< "Начинаем разбор разархивированных директорий." << endl;
//Разбор разархивированной директории
    vector<string> vectorJsonFilesPath;
    vector<string> errorJsonPatch;
    const unordered_set<string> json_extensions{ ".json" };
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
            cout <<endl<<"Seaching json "<< num << ": "<< patchJsonFile << endl;
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

string nameFromPath(path Patch){
    return Patch.filename();
}

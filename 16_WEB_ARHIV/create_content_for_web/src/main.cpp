﻿#include "main.h"
string WEB_content{"../../CONTENT/content_for_web/"};

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        cout << "Для работы приложения требуется параметр (путь к папке с архивом)"<< endl;
        return 0;
    }
    const path archiv_path_zip{ argv[1] };
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
        //тут пробел нужно заменить на "\"
        auto posBlank = patch.find(" ");
        patch.insert(posBlank,"\\");
        string name = entry.path().filename();
        vectorZipFilesPath.push_back(patch);
        name.pop_back();
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
        int num = 0;
        for (auto i = 0; i < vectorZipFilesPath.size();++i ){
            ++num;
            cout << endl<< num << ": "<< vectorZipFilesPath[i] << endl;
            extractZip(vectorZipFilesPath[i], vectorZipFilesName[i], pathToExtractDirectory); //распаковка одного zip файла
        }
    }
    cout<< endl<< "Всего разархивировано zip файлов: " << vectorZipFilesPath.size()<< endl;

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
            if(!parseJSON(patchJsonFile)) //парсинг одного файла JSON
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

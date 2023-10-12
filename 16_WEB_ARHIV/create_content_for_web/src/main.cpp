#include "main.h"

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        cout << "Для работы приложения требуется параметр (путь к папке с архивом)"<< endl;
        return 0;
    }

// 7z x ~/SOFT/Github/GIT/16_WEB_ARHIV/Ниокр-Актуальные\ документы/ГРЛМ.301122.007.zip -y -o"/home/evg/SOFT/Github/GIT/16_WEB_ARHIV/EXTRACT"


//Разархивирование исходного контента
    const path archiv_path_zip{ argv[1] };
    vector<string> vectorZipFilesPath;
    vector<string> errorZipExtract;
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
        ifstream file{ entry.path() };
        string patch = entry.path().string();
        vectorZipFilesPath.push_back(patch);
      } catch(const exception& e) {
        cerr << "Error reading " << entry.path().string() << ": " << e.what() << endl;
      }
    }
    int errorExtractZip = 0;
    if (vectorZipFilesPath.size()>0)
    {
        int num = 0;
        for (auto & vectorZipFilesPath : vectorZipFilesPath){
            ++num;
            cout << endl<< num << ": "<< vectorZipFilesPath << endl;
            if(!extractZip(vectorZipFilesPath)) //распаковка одного zip файла
            {
                cout << "ОШИБКА ZIP" << endl;
                ++errorExtractZip;
                errorZipExtract.push_back(vectorZipFilesPath);
            }
        }
    }
    cout<< endl<< "Всего разархивировано zip файлов: " << vectorZipFilesPath.size()
       << endl << "Ошибок разбора: " << errorExtractZip<< endl;
    for (auto & zipError : errorZipExtract)
        cout <<  zipError << endl;




//Разбор разархивированной директории
    const path archiv_path_extracted{""};
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
        ifstream file{ entry.path() };
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

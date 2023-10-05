#include "main.h"

int main(int argc, char *argv[])
{
//    const path archiv_path{ R"(/home/evg/SOFT/Github/GIT/15_Документы_НИОКР/temp_arсhive)" };
//    const path archiv_path{ R"(/home/evg/Desktop/temp_arсhive)" };
    const path archiv_path{ R"(C:\Users\Professional\Documents\GIT1\15_Документы_НИОКР\temp_arсhive)" };
    const unordered_set<string> pe_extensions{ ".json" };
    cout << endl <<"Patch for json search: "<<archiv_path << endl;
    vector<string> vectorJsonFiles;
    vector<string> errorJsonPatch;

    auto iterator = recursive_directory_iterator{ archiv_path, directory_options::skip_permission_denied };
    for(const auto& entry : iterator) {
      try {
        if(!entry.is_regular_file())
          continue;
        const auto& extension = entry.path().extension().string();
        const auto is_pe = pe_extensions.find(extension) != pe_extensions.end();
        if(!is_pe)
          continue;
        ifstream file{ entry.path() };
        string patch = entry.path().string();
        vectorJsonFiles.push_back(patch);
      } catch(const exception& e) {
        cerr << "Error reading " << entry.path().string() << ": " << e.what() << endl;
      }
    }
    int error = 0;
    if (vectorJsonFiles.size()>0)
    {
        int num = 0;
        for (auto & patchJsonFile : vectorJsonFiles){
            ++num;
            cout << endl<< num << ": "<< patchJsonFile << endl;
            if(!parseJSON(patchJsonFile)) //парсинг одного файла JSON
            {
                cout << "ОШИБКА JSON ФАЙЛА" << endl;
                ++error;
                errorJsonPatch.push_back(patchJsonFile);
            }
        }
    }

    cout<< endl<< "Всего разобрано json файлов: " << vectorJsonFiles.size()
       << endl << "Ошибок разбора: " << error<< endl;
    for (auto & patchJsonError : errorJsonPatch)
        cout <<  patchJsonError << endl;


}

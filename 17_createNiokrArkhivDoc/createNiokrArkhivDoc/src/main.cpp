#include "main.h"
#include <curses.h>
#include <stdio.h>
#include <sys/select.h>
#include <termios.h>
#include <stropts.h>
#include <unistd.h>
#if 0
Деплой windows
cd C:\Qt5\5.15.2\mingw81_64\bin
windeployqt.exe C:\Users\Professional\Desktop\createNiokrArkhivDoc\createNiokrArkhivDoc.exe
cd C:\Users\Professional\Desktop\createNiokrArkhivDoc
createNiokrArkhivDoc.exe C:\Users\Professional\Desktop\temp_arсhive
Скопировать в папку с exe 2 файла   busybox.exe    grep.exe  из папки bin пакета  w64devkit-master или прописать путь
C:\w64devkit-master\w64devkit\bin  в path
Скопировать в папку с exe 2 файла  7z.exe  7z.dll
Или прописать путь к 7z
 #endif
map<string,string> allData{};



int main(int argc, char *argv[])
{
    if (argc == 2) {
        cout << string{"Передается в качестве параметра путь к папке: "}.append(argv[1]) << endl<< endl;
    }
    else {
        cout << "Ошибка запуска. Необходимо передать путь к папке с одной единицей хранения!" << endl;
        return 0;
    }
    const path archiv_path{ argv[1] };
    const unordered_set<string> pe_extensions{ ".json" };
//    cout << endl <<"Patch for json search: "<<archiv_path << endl;
    vector<string> vectorJsonFiles;

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
        if (vectorJsonFiles.size()>1)
        {
            cout << "ОШИБКА - в переданной папке более одного json файла!!!" << endl;
            return 0;
        }
        int num = 0;
        for (auto & patchJsonFile : vectorJsonFiles){
            ++num;
//            cout << endl<< num << ": "<< patchJsonFile << endl;
            if(!parseJSON(patchJsonFile)) //парсинг одного файла JSON
            {
                cout << "ОШИБКА JSON ФАЙЛА" << endl;
                return 0;
            }
        }
    }
//Тут второй этап - ответы на вопросы
    cout << "в ИУЛ в графе Обозначение документа содержится надпись " << allData["oboznachenieIkodDokumenta"] << " ? . . . . . " ;

 initscr();
char s;
int c = getch();


return 0;

}

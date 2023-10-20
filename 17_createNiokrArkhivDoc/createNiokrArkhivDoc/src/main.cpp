#include "main.h"
#include <curses.h>
#include <ncursesw/ncurses.h>
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

ncurses
sudo apt-get install libncurses5-dev libncursesw5-dev
https://code-live.ru/post/terminal-mode-management-ncurses/
https://ru.stackoverflow.com/questions/1263580/ncurses-ввод-и-вывод-русского-символа-на-экран
 #endif
map<string,string> allData{};

bool enter(uint num, string qw)
{
    initscr();
    mvprintw(num*2-2, 0, qw.c_str());
    initscr();
    noecho();
    halfdelay(100);         //Устанавливаем ограничение по времени ожидания getch() в 10 сек
    bool ret = true;
    bool ex = false;
    uint numCharEntered = 0;
    wint_t tmp;
    wint_t c1;
    wint_t c2;
    wint_t c3;
    while ( !ex ) {
        int rc = get_wch(&tmp);
        if (rc==OK) {
          ++numCharEntered;
        }
        switch (numCharEntered) {
        case 1:
            c1 = tmp;
            if ((c1 != L'д') && (c1 != L'н')){
            mvprintw(num*2-2, 0, qw.c_str());
            mvprintw(num*2-1, 0, "Введите да  или  нет");
            numCharEntered = 0;
            }
            if ((c1 == L'д') || (c1 == L'н')){
            mvprintw(num*2-2, 0, qw.c_str());
            mvprintw(num*2-1, 0, "                    \b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b");
            if (c1 == L'д')
                mvprintw(num*2-1, 0, "д");
            if (c1 == L'н')
                mvprintw(num*2-1, 0, "н");
            }
            break;

        case 2:
            c2 = tmp;
            if ((c1 == L'д') && (c2 == L'а')){
                mvprintw(num*2-1, 1, "а");
            ex = true;
            }
            if ((c1 == L'д') && (c2 != L'а')){
            mvprintw(num*2-2, 0, qw.c_str());
            mvprintw(num*2-1, 0, "Введите да  или  нет");
            numCharEntered = 0;
            }
            if ((c1 == L'н') && (c2 == L'е')){
            mvprintw(num*2-1, 1, "е");
            }
            break;
        case 3:
            c3 = tmp;
            if ((c1 == L'н') && (c2 == L'е') && (c3 == L'т')){
            ex = true;
            ret = false;
            }
            if ((c1 == L'н') && (c2 != L'e')){
            mvprintw(num*2-2, 0, qw.c_str());
            mvprintw(num*2-1, 0, "Введите да  или  нет");
            numCharEntered = 0;
            }
            if ((c1 == L'н') && (c2 == L'e') && (c3 != L'т')){
            mvprintw(num*2-2, 0, qw.c_str());
            mvprintw(num*2-1, 0, "Введите да  или  нет");
            numCharEntered = 0;
            }
            break;
        default:
            break;
        }
    }
    refresh(); //Выводим на настоящий экран
    endwin();
 return ret;
}

bool answers(){
    if (!enter(1, string("1. В ИУЛ в графе Обозначение документа содержится надпись ").append(allData["oboznachenieIkodDokumenta"]).append(" ? \n")))
        return false;
    if (!enter(2, string("2. В ИУЛ в графе Наименование изделия содержится надпись ").append(allData["naimenovanieIzdeliya"]).append(" ? \n")))
        return false;
    if (!enter(3, string("3. В ИУЛ в графе Наименование документа содержится надпись ").append(allData["naimenovanieDokumenta"]).append(" ? \n")))
        return false;
    if (!enter(4, string("4. В ИУЛ в графе Номер последнего изменения указан номер ").append(allData["changeNumStr"]).append(" ? \n")))
        return false;
     return true;
}

int main(int argc, char *argv[])
{
    setlocale (LC_ALL,""); //для ncurses
    system("clear");
    if (argc == 2) {
        cout << string{"Передан в качестве параметра путь к папке: "}.append(argv[1]) << endl<< endl;
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
    if (!answers()){
        cout << "\nИсправьте ИУЛ !!!\n" <<endl;
        return 0;
    }
//Сохраняем лог ответов




return 0;

}

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

void mainQw(uint num, vector<string> & qw)
{
    mvprintw(num*2-2, 0, qw[0].c_str());
    attron(COLOR_PAIR(1));
    printw(qw[1].c_str());
    attron(COLOR_PAIR(3));
    printw(qw[2].c_str());
    attron(COLOR_PAIR(2));
    printw(qw[3].c_str());
    attron(COLOR_PAIR(3));
    printw(qw[4].c_str());
}

bool enter(uint num, string qw1, string qw2, string qw3, string qw4, string qw5)
{
    vector<string>qw{qw1, qw2, qw3, qw4, qw5};
    initscr();
    start_color();
    init_pair(1, COLOR_CYAN, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_WHITE, COLOR_BLACK);
    attron(COLOR_PAIR(3));
    mainQw(num, qw);

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
            mainQw(num, qw);
            mvprintw(num*2-1, 0, "Введите да  или  нет");
            numCharEntered = 0;
            }
            if ((c1 == L'д') || (c1 == L'н')){
            mainQw(num, qw);
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
            mainQw(num, qw);
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
            mainQw(num, qw);
            mvprintw(num*2-1, 0, "Введите да  или  нет");
            numCharEntered = 0;
            }
            if ((c1 == L'н') && (c2 == L'e') && (c3 != L'т')){
            mainQw(num, qw);
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
    if (!enter(1, "1. В ИУЛ в графе ", "Обозначение документа", " содержится надпись ", allData["oboznachenieIkodDokumenta"], " ? \n"))
        return false;
    if (!enter(2, "2. В ИУЛ в графе ", "Наименование изделия", " содержится надпись ", allData["naimenovanieIzdeliya"], " ? \n"))
        return false;
    if (!enter(3, "3. В ИУЛ в графе ", "Наименование документа", " содержится надпись ", allData["naimenovanieDokumenta"], " ? \n"))
        return false;
    if (!enter(4, "4. В ИУЛ в графе ", "Номер последнего изменения", " указан номер ", allData["changeNumStr"], " ? \n"))
        return false;
    if (!enter(5, "5. В ИУЛ в графе ", "Подлинник", " Указано имя ", string(allData["oboznachenieIkodDokumenta"]).append(".pdf "), " ? \n"))//может быть zip !!!
        return false;
    if (!enter(6, "6. В ИУЛ в графе ", "Содержательные части", " содержится надпись ", string(allData["oboznachenieIkodDokumenta"]).append(".zip\\Contents"), " ? \n"))
        return false;
    if (!enter(7, "7. В ИУЛ в графе ", "CRC32 Подлинника", " указано значение ", allData["controlSummOrigin"], " ? \n"))
        return false;
    if (!enter(8, "8. В ИУЛ в графе ", "CRC32 Содержательных частей", " указано значение ", allData["controlSummParts"], " ? \n"))
        return false;
    if (!enter(9, "9. В ИУЛ в графе ", "Информационно-удостоверяющий лист", " содержится надпись ", allData["infoOrderList"], " ? \n"))
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
    cout<< "Ответы на вопросы ... OK" <<endl;
//Сохраняем лог ответов




return 0;

}

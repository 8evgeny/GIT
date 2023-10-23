#include "main.h"
#include <curses.h>
#include <ncursesw/ncurses.h>
#include <boost/program_options.hpp>
namespace po = boost::program_options;
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

void colorMsg(uint num, vector<string> & qw)
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

void logging()
{

}

bool workInConsole(uint num, string qw1, string qw2, string qw3, string qw4, string qw5)
{
    vector<string>qw{qw1, qw2, qw3, qw4, qw5};
    initscr();
    start_color();
    init_pair(1, COLOR_YELLOW, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_WHITE, COLOR_BLACK);
    attron(COLOR_PAIR(3));
    colorMsg(num, qw);

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
            colorMsg(num, qw);
            mvprintw(num*2-1, 0, "Введите да  или  нет");
            numCharEntered = 0;
            }
            if ((c1 == L'д') || (c1 == L'н')){
            colorMsg(num, qw);
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
            colorMsg(num, qw);
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
            colorMsg(num, qw);
            mvprintw(num*2-1, 0, "Введите да  или  нет");
            numCharEntered = 0;
            }
            if ((c1 == L'н') && (c2 == L'e') && (c3 != L'т')){
            colorMsg(num, qw);
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
//Логгирование ответов
    logging();
 return ret;
}

bool answers(map<string,string> & allData){
    if (!workInConsole(1, "1. В ИУЛ в графе ", "Обозначение документа", " содержится надпись ", allData["oboznachenieIkodDokumenta"], " ? \n"))
        return false;
    if (!workInConsole(2, "2. В ИУЛ в графе ", "Наименование изделия", " содержится надпись ", allData["naimenovanieIzdeliya"], " ? \n"))
        return false;
    if (!workInConsole(3, "3. В ИУЛ в графе ", "Наименование документа", " содержится надпись ", allData["naimenovanieDokumenta"], " ? \n"))
        return false;
    if (!workInConsole(4, "4. В ИУЛ в графе ", "Номер последнего изменения", " указан номер ", allData["changeNumStr"], " ? \n"))
        return false;
    if (!workInConsole(5, "5. В ИУЛ в графе ", "Подлинник", " Указано имя ", string(allData["oboznachenieIkodDokumenta"]).append(".pdf "), " ? \n"))//может быть zip !!!
        return false;
    if (!workInConsole(6, "6. В ИУЛ в графе ", "Содержательные части", " содержится надпись ", string(allData["oboznachenieIkodDokumenta"]).append(".zip\\Contents"), " ? \n"))
        return false;
    if (!workInConsole(7, "7. В ИУЛ в графе ", "CRC32 Подлинника", " указано значение ", allData["controlSummOrigin"], " ? \n"))
        return false;
    if (!workInConsole(8, "8. В ИУЛ в графе ", "CRC32 Содержательных частей", " указано значение ", allData["controlSummParts"], " ? \n"))
        return false;
    if (!workInConsole(9, "9. В ИУЛ в графе ", "Информационно-удостоверяющий лист", " содержится надпись ", allData["infoOrderList"], " ? \n"))
        return false;
     return true;
}

map<string, string> readConfig(const char* conf_file) {
    map<string, string> config{};
    cout<<"reading config.ini file\n"<<endl;
    po::variables_map vm;
    po::options_description patches("patchesToDirectories");
    patches.add_options()
          ("patchesToDirectories.niokrActualDocs", po::value<string>(), "")
          ("patchesToDirectories.niokrPoOboznacheniyam", po::value<string>(), "")
          ("patchesToDirectories.niokrOldDocs", po::value<string>(), "");
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
  config["niokrOldDocs"] = vm["patchesToDirectories.niokrOldDocs"].as<string>();
  cout << "niokrActualDocs:       \t" << config["niokrActualDocs"] << endl;
  cout << "niokrPoOboznacheniyam: \t" << config["niokrPoOboznacheniyam"] << endl;
  cout << "niokrOldDocs:          \t" << config["niokrOldDocs"] << endl<<endl;
  return config;
}

int main(int argc, char *argv[])
{
    setlocale (LC_ALL,""); //для ncurses
    system("clear");
    if (argc == 2) {
//        cout << string{"Передан в качестве параметра путь к папке: "}.append(argv[1]) << endl<< endl;
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
    map<string,string> allData{};
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
            if(!parseJSON(patchJsonFile, allData)) //парсинг одного файла JSON
            {
                cout << "ОШИБКА JSON ФАЙЛА" << endl;
                return 0;
            }
        }
    }
//Тут второй этап - ответы на вопросы

//    if (!answers(allData)){
//        cout << "\nИсправьте ИУЛ !!!\n" <<endl;
//        return 0;
//    }
    cout<< "Ответы на вопросы ... OK" <<endl;

//Операции с файлами
    auto config = readConfig("../config.ini"); //Читаем конфиг

    if (allData["changeNumStr"] != "0"){//Изм не 0
//Заменяем неактуальный документ
//Копируем заменяемый zip из папки актуальных документов в папку tmp
        string createTmpDir{"mkdir tmpDir 2>/dev/null"};
        system(createTmpDir.c_str());
        string clearTmpDir{"cd tmpDir && rm -rf * 2>/dev/null"};
        system(clearTmpDir.c_str());
        string moveZipToTmpDir{"cp "};
        moveZipToTmpDir.append(config["niokrActualDocs"]).append("/").append(allData["oboznachenieIkodDokumenta"]).append(".zip").append(" tmpDir");
        system(moveZipToTmpDir.c_str());

//Разархивируем в папку tmpDir/tmp и удаляем заменяемый zip
        string exteractZip{"cd tmpDir && 7z x -bsp0 -bso0 "};
        exteractZip.append(allData["oboznachenieIkodDokumenta"]).append(".zip -o'tmp'");
        system(exteractZip.c_str());
        string deleteZip{"rm  "};
        deleteZip.append(" tmpDir/").append(allData["oboznachenieIkodDokumenta"]).append(".zip");
        system(deleteZip.c_str());
//Проверяем что номер изм  на 1 меньше текущего
        map<string,string> tmpData{};
        string jsonPath{"tmpDir/tmp/index.json"};
        if(!parseJSON(jsonPath, tmpData)) {
            cout << "json error" << endl;
            return 0;
        }
        int izmCurrent = atoi(allData["changeNumStr"].c_str());
        int izmOld = atoi(tmpData["changeNumStr"].c_str());
        if (izmCurrent - izmOld != 1) {
            cout << "ОШИБКА! Номер изменения отличается больше чем на единицу" << endl;
            return 0;
        }
        cout << "Проверка номера изм. ... OK" << endl;
//Удаляем zip из папки актуальных документов
        string deleteOldZip{"rm "};
        deleteOldZip.append(config["niokrActualDocs"]).append("/").append(allData["oboznachenieIkodDokumenta"]).append(".zip");
        system(deleteOldZip.c_str());

//В json меняем true на false
        string replaceBool{"cd tmpDir/tmp && sed -i 's/true/false/' index.json"};
        system(replaceBool.c_str());

//Архивируем в папку неактуальные , добавляя в конце изм номер
        cout<<"В папке Ниокр-Неактуальные_документы  создан новый документ: "  << allData["oboznachenieIkodDokumenta"] <<".изм."<< tmpData["changeNumStr"]<<".zip" << endl;
        string cmd7zip{"7z a -tzip -mx7 -bsp0 -bso0 "};
        cmd7zip.append(config["niokrOldDocs"]).append("/").append(allData["oboznachenieIkodDokumenta"]).append(".изм.").append(tmpData["changeNumStr"]).append(".zip");
        cmd7zip.append(" ./tmpDir/tmp/*");
        system(cmd7zip.c_str());

    }
//Если изм 0 то попадаем сразу сюда
//Архивируем в папку актуальные 7z a -tzip -mx7 -bsp0 -bso0 ~/SOFT/Github/GIT/17_createNiokrArkhivDoc/Ниокр-Актуальные_документы/11.zip ~/SOFT/Github/GIT/17_createNiokrArkhivDoc/createNiokrArkhivDoc/ГРЛМ.301122.007СБ/*
    cout<<"В папке Ниокр-Актуальные_документы  создан новый документ: "  << allData["oboznachenieIkodDokumenta"] <<".zip" << endl;
    string cmd7zip{"7z a -tzip -mx7 -bsp0 -bso0 "};
    cmd7zip.append(config["niokrActualDocs"]).append("/").append(allData["oboznachenieIkodDokumenta"]).append(".zip");
    cmd7zip.append(" ");
    cmd7zip.append(string(argv[1]).append("/*"));
    system(cmd7zip.c_str());

//Удаляем старый подлинник с таким-же именем (если есть) Переименовываем pdf в PDF и копируем подлинник PDF в папку Ниокр-Документы_по_обозначениям
    cout<<"В папку Ниокр-Документы_по_обозначениям скопирован подлинник: " <<  allData["oboznachenieIkodDokumenta"]<<".PDF" <<  endl;
    string cmdDelateOriginPDF{"rm -f "};
    string cmdDelateOriginpdf{"rm -f "};
    cmdDelateOriginPDF.append(config["niokrPoOboznacheniyam"]).append("/").append(allData["oboznachenieIkodDokumenta"]).append(".PDF 2> /dev/null");
    cmdDelateOriginpdf.append(config["niokrPoOboznacheniyam"]).append("/").append(allData["oboznachenieIkodDokumenta"]).append(".pdf 2> /dev/null");
    system(cmdDelateOriginPDF.c_str());
    system(cmdDelateOriginpdf.c_str());
    string remaneOriginpdfToPDF{"mv "};
    remaneOriginpdfToPDF.append(archiv_path).append("/Contents/").append(allData["oboznachenieIkodDokumenta"]).append(".pdf ").append(archiv_path).append("/Contents/").append(allData["oboznachenieIkodDokumenta"]).append(".PDF 2> /dev/null");
    system(remaneOriginpdfToPDF.c_str());
    string copyOriginPDF{"cp "};
    copyOriginPDF.append(archiv_path).append("/Contents/").append(allData["oboznachenieIkodDokumenta"]).append(".PDF ").append(config["niokrPoOboznacheniyam"]);
    system(copyOriginPDF.c_str());

return 0;

}

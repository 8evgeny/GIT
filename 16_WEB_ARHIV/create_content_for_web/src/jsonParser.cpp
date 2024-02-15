#include "main.h"
extern string WEB_content;
extern uint numContent;
uint numFolderForWebContent{1};
QString namePDF;

    ostream& operator<<(ostream &os, const chrono::time_point<chrono::system_clock> &t){
        const auto tt   (chrono::system_clock::to_time_t(t));
        const auto loct (localtime(&tt));
        return os << put_time(loct, "%Y-%m-%d");//%c
    }

    using days = chrono::duration<chrono::hours::rep, ratio_multiply<chrono::hours::period, ratio<24>>>;
    constexpr days operator ""_days(unsigned long long d){
        return days{d};
    }

bool parseJSON(string & patchToFile, const path & archiv_path_zip){ //archiv_path_zip передаю чтобы получить через нее доступ к папке с ИУЛами
    QFile file(QString::fromStdString(patchToFile));
    file.open(QIODevice::ReadOnly);
    QString jsonData = file.readAll();
    file.close();

    //Переменные
    string naimenovanieIzdeliya{""};
    string naimenovanieDokumenta{""};
    string oboznachenieIkodDokumenta{""};                     //1
    quint32 crc32; //СКС32 PDF файла
    string crc32Main;
    string crc32Contents; //СКС32 папки Contents
    int numberSheets{-1};
    string company{""};
    string creater{""};
    auto createdData = chrono::system_clock::now();
    string createdDataStr{""};
    string infoOrderList{""};
    int changeNum{-1};
    string changeNumStr{""};                                   //2
    string changeNotificationNum{""};                          //3
    auto notificationData = chrono::system_clock::now();
    string notificationDataStr{""};
    string inventoryNumOriginal{""};
    auto storageData = chrono::system_clock::now();
    string storageDataStr{""};                                 //4
    string litera{""};                                         //5
    int dokumentCode{-1};
    int version{-1};
    string algorithm{""};
    string controlSummOrigin{""};                               //6
    string controlSummParts{""};                                //7
    string software{""};
    int volumeInSheets{-1};
    bool isValid{false};
    string tags{""};

    QRegExp blankStr("^$");//пустая строка
    QRegExp dataStr("^20\\d{2}-(0[1-9]|1[0-2])-(0[1-9]|[12][0-9]|3[01])$");
    QRegExp number2dig("^[0-9]{1}[0-9]?$");
    QRegExp number3dig("^[0-9]{0,3}$");
    QRegExp crc32Str("^[0-9ABCDEF]{8}$");
    QRegExp softStr("^"
                    "(Solidworks Professional 2018 x64 Edition SP3.0)|"
                    "(КОМПАС-3D 21.0.0.1158 /(64-разрядная версия/))|"
                    "(Altium Designer 20.0.12 /(Build 288/))|"
                    "(Libre Office Writer 7.5.3.2)|"
                    "(Inkscape 1.2)|"
                    "(NanoCAD 23.0.6169.4115 /(Сборка 6276/))|"
                    "Altium Designer Version 19.0.15 /(BUILD 446/)|"
                    "(Microsoft Word)"
                    "$");
    QRegExp iulStr("^"
                   "(ГРЛМ.[0-9]{6}.[0-9]{3}[А-Я][А-Я0-9]+)-УЛ|"
                   "(ГРЛМ.[0-9]{6}.[0-9]{3}УЛ)|"
                   "(ГРЛМ.[0-9]{6}.[0-9]{3}-[0-9]{2,3}[А-Я][А-Я0-9]+)-УЛ|"
                   "$");
    Document document;
    if (!document.Parse(jsonData.toStdString().c_str()).HasParseError())
    {
        if (document.IsObject())
        {
            if (document.HasMember("Реквизиты документа по ГОСТ 2.104"))
            {
                cout << "   Реквизиты документа по ГОСТ 2.104:\n";
                const   Value & requisites = document["Реквизиты документа по ГОСТ 2.104"];
                if (requisites.HasMember("Наименование изделия")){
                    naimenovanieIzdeliya = requisites["Наименование изделия"].GetString();
                    if(blankStr.exactMatch(QString::fromStdString(naimenovanieIzdeliya))) return false;
                    cout << "\tНаименование изделия: " << naimenovanieIzdeliya << "\n";
                }else return false;
                if (requisites.HasMember("Наименование документа")){
                    naimenovanieDokumenta = requisites["Наименование документа"].GetString();
                    if(blankStr.exactMatch(QString::fromStdString(naimenovanieDokumenta))) return false;
                    cout << "\tНаименование документа: " << naimenovanieDokumenta << "\n";
                }else return false;
                if (requisites.HasMember("Обозначение и код документа")){
                    oboznachenieIkodDokumenta = requisites["Обозначение и код документа"].GetString();
                    if(blankStr.exactMatch(QString::fromStdString(oboznachenieIkodDokumenta))) return false;
      //вычисляем CRC32 PDF файла
      string appPdf = oboznachenieIkodDokumenta;
      string chopped = patchToFile;
      namePDF = QString::fromStdString(chopped).chopped(10)+QString("Contents/")+QString::fromStdString(appPdf.append(".PDF"));
      QString directoryPDF = QString::fromStdString(chopped).chopped(10)+QString("Contents/");
      string printDir = "ls -l " + directoryPDF.toStdString();
      system(printDir.c_str());
      crc32 = CRC32(namePDF);
      cout << "\tОбозначение и код документа: " << oboznachenieIkodDokumenta ;
      if(crc32 == -1)
      {
          printf("                      \tОтсутствует PDF файл  %s.PDF !!!\n", oboznachenieIkodDokumenta.c_str());
          return false;
      }
      printf("                      \t(посчитан CRC32: %.8X)\n", crc32);
      char buf[20];
      sprintf(buf,"%.8X\n", crc32);
      string tmp{buf};
      crc32Main = tmp;

          //вычисляем CRC32 папки Contents
      string chopped1 = patchToFile;
      QString nameDirectory = QString::fromStdString(chopped1).chopped(10)+QString("Contents");
      crc32Contents = CRC32Contents(nameDirectory);
      printf("                      \t\t\t\t\t\t\t\t(посчитан CRC32 папки Contents: %s)\n", crc32Contents.c_str());

                }else return false;
                if (requisites.HasMember("Общее количество листов документа")){
                    auto type = requisites["Общее количество листов документа"].GetType();
                    if (type == rapidjson::kStringType)
                       numberSheets =  atoi(requisites["Общее количество листов документа"].GetString());
                    if (type == rapidjson::kNumberType)
                       numberSheets =  requisites["Общее количество листов документа"].GetInt();
                    if(!number3dig.exactMatch(QString::number(numberSheets))) return false;
                    cout << "\tОбщее количество листов документа: " << numberSheets << "\n";
                }else return false;
                if (requisites.HasMember("Наименование или код организации")){
                    company = requisites["Наименование или код организации"].GetString();
                    if(company != "ООО «Группа индустриальных технологий»") return false;
                    cout << "\tНаименование или код организации: " << company << "\n";
                }else return false;
                if (requisites.HasMember("Сведения о подписании документа")){
                    cout << "\tСведения о подписании документа:\n";
                    const   Value & infoAboutSigning = requisites["Сведения о подписании документа"];
                    if (infoAboutSigning.HasMember("Разработал")){
                        creater = infoAboutSigning["Разработал"].GetString();
                        if(blankStr.exactMatch(QString::fromStdString(creater))) return false;
                        cout << "\t\tРазработал: " << creater << "\n";
                    }else return false;
                    if (infoAboutSigning.HasMember("Дата")){
                        createdDataStr = infoAboutSigning["Дата"].GetString();
                        if(!dataStr.exactMatch(QString::fromStdString(createdDataStr))) return false;
                        stringstream ss(createdDataStr);
                        tm tm = {};
                        ss >> get_time(&tm, "%Y-%m-%d");
                        createdData = chrono::system_clock::from_time_t(mktime(&tm));
                        cout << "\t\tДата: " << createdData << "\n";
                    }else return false;
                    if (infoAboutSigning.HasMember("Информационно-удостоверяющий лист")){
                        infoOrderList = infoAboutSigning["Информационно-удостоверяющий лист"].GetString();
                        if(!iulStr.exactMatch(QString::fromStdString(infoOrderList))) return false;
                        cout << "\t\tИнформационно-удостоверяющий лист: " << infoOrderList << "\n";
                    }else return false;
                }else return false;
                if (requisites.HasMember("Номер изменения")){
                    auto type = requisites["Номер изменения"].GetType();
                    if (type == rapidjson::kStringType)
                        changeNum =  atoi(requisites["Номер изменения"].GetString());
                    if (type == rapidjson::kNumberType)
                        changeNum =  requisites["Номер изменения"].GetInt();
                        if(!number2dig.exactMatch(QString::number(changeNum))) return false;
                    cout << "\tНомер изменения: " << changeNum << "\n";
                    changeNumStr = to_string(changeNum);
                }else return false;
                if (requisites.HasMember("Номер извещения об изменении")){
                    changeNotificationNum = requisites["Номер извещения об изменении"].GetString();
                    if(blankStr.exactMatch(QString::fromStdString(changeNotificationNum))) return false;
                    cout << "\tНомер извещения об изменении: " << changeNotificationNum << "\n";
                }else return false;
                if (requisites.HasMember("Дата извещения об изменении")){
                    notificationDataStr = requisites["Дата извещения об изменении"].GetString();
                    if(!dataStr.exactMatch(QString::fromStdString(notificationDataStr))) return false;
                    stringstream ss(notificationDataStr);
                    tm tm = {};
                    ss >> get_time(&tm, "%Y-%m-%d");
                    notificationData = chrono::system_clock::from_time_t(mktime(&tm));
                    cout << "\tДата извещения об изменении: " << notificationData << "\n";
                }else return false;
                if (requisites.HasMember("Инвентарный номер подлинника")){
                    inventoryNumOriginal = requisites["Инвентарный номер подлинника"].GetString();
                    if(blankStr.exactMatch(QString::fromStdString(inventoryNumOriginal))) return false;
                    cout << "\tИнвентарный номер подлинника: " << inventoryNumOriginal << "\n";
                }else return false;
                if (requisites.HasMember("Дата приемки на хранение")){
                    storageDataStr = requisites["Дата приемки на хранение"].GetString();
                    if(!dataStr.exactMatch(QString::fromStdString(storageDataStr))) return false;
                    stringstream ss(storageDataStr);
                    tm tm = {};
                    ss >> get_time(&tm, "%Y-%m-%d");
                    storageData = chrono::system_clock::from_time_t(mktime(&tm));
                    cout << "\tДата приемки на хранение: " << storageData << "\n";
                }else return false;
                if (requisites.HasMember("Литера")){
                    litera = requisites["Литера"].GetString();
                    if (litera == "")
                        cout << "\tЛитера: " << "\"\"" << "\n";
                    else
                        cout << "\tЛитера: " << litera << "\n";
                }else return false;
                if (requisites.HasMember("Код документа в зависимости от характера использования")){
                    auto type = requisites["Код документа в зависимости от характера использования"].GetType();
                    if (type == rapidjson::kStringType)
                        dokumentCode =  atoi(requisites["Код документа в зависимости от характера использования"].GetString());
                    if (type == rapidjson::kNumberType)
                        dokumentCode =  requisites["Код документа в зависимости от характера использования"].GetInt();
                    if(!number2dig.exactMatch(QString::number(dokumentCode))) return false;
                    cout << "\tКод документа в зависимости от характера использования: " << dokumentCode << "\n";
                }
            }//Реквизиты документа
            else return false;

            if (document.HasMember("Сервисные данные")){
                cout << "   Сервисные данные:\n";
                const   Value & serviceData = document["Сервисные данные"];
                if (serviceData.HasMember("Версия формата реквизитной части")){
                    auto type = serviceData["Версия формата реквизитной части"].GetType();
                    if (type == rapidjson::kStringType)
                        version =  atoi(serviceData["Версия формата реквизитной части"].GetString());
                    if (type == rapidjson::kNumberType)
                        version =  serviceData["Версия формата реквизитной части"].GetInt();
                    if(!number2dig.exactMatch(QString::number(version))) return false;
                    cout << "\tВерсия формата реквизитной части: " << version << "\n";
                }else return false;
                if (serviceData.HasMember("Алгоритм расчета контрольной суммы")){
                    algorithm = serviceData["Алгоритм расчета контрольной суммы"].GetString();
                    if(blankStr.exactMatch(QString::fromStdString(algorithm))) return false;
                    cout << "\tАлгоритм расчета контрольной суммы: " << algorithm << "\n";
                }else return false;
                if (serviceData.HasMember("Значение контрольной суммы подлинника")){
                    controlSummOrigin = serviceData["Значение контрольной суммы подлинника"].GetString();
                if(!crc32Str.exactMatch(QString::fromStdString(controlSummOrigin))) return false;

                    //Сравниваем с расчитанным CRC32
                    char buf[20];
                    sprintf (buf,"%.8X", crc32);
                    string calculateCRC32{buf};
                    cout << "\tЗначение контрольной суммы подлинника: " << controlSummOrigin ;
                    if(controlSummOrigin == calculateCRC32) {
                        cout << "                  \t(Контрольные суммы совпадают)"<<endl; }
                    else { cout << "                  \t(Контрольные суммы не совпадают !!!)"<<endl;
                        return false;}

                }else return false;
                if (serviceData.HasMember("Значение контрольной суммы содержательных частей")){
                controlSummParts = serviceData["Значение контрольной суммы содержательных частей"].GetString();
                if(!crc32Str.exactMatch(QString::fromStdString(controlSummParts))) return false;
                cout << "\tЗначение контрольной суммы содержательных частей: " << controlSummParts ;
                if(controlSummParts == crc32Contents) {
                    cout << "  \t\t(Контрольные суммы содержательных частей совпадают)"<<endl; }
                else { cout << "  \t\t(Контрольные суммы содержательных частей не совпадают !!!)"<<endl;
                    return false;}

                }else return false;
                if (serviceData.HasMember("Программное обеспечение для редактирования исходных данных")){
                    software = serviceData["Программное обеспечение для редактирования исходных данных"].GetString();
                    if(!softStr.exactMatch(QString::fromStdString(software))) return false;
                    cout << "\tПрограммное обеспечение для редактирования исходных данных: " << software << "\n";
                }else return false;
                if (serviceData.HasMember("Объем в листах А4")){
                    auto type = serviceData["Объем в листах А4"].GetType();
                    if (type == rapidjson::kStringType)
                        volumeInSheets =  atoi(serviceData["Объем в листах А4"].GetString());
                    if (type == rapidjson::kNumberType)
                        volumeInSheets =  serviceData["Объем в листах А4"].GetInt();
                    if(!number3dig.exactMatch(QString::number(volumeInSheets))) return false;
                    cout << "\tОбъем в листах А4: " << volumeInSheets << "\n";
                }else return false;
                if (serviceData.HasMember("Документ действителен")){
                    auto type = serviceData["Документ действителен"].GetType();
                    if (type == rapidjson::kTrueType || rapidjson::kFalseType)
                       isValid = serviceData["Документ действителен"].GetBool();
                    else return false;
                    cout << "\tДокумент действителен: " << boolalpha << isValid << "\n";
                }else return false;
                if (serviceData.HasMember("Теги")){
                    tags = serviceData["Теги"].GetString();
                    if (tags == "")
                        cout << "\tТеги: " << "\"\"" << "\n";
                    else
                        cout << "\tТеги: " << tags << "\n\n";
                }else return false;
            } else return false;//Сервисные данные
        }//document.IsObject()
    }//Parse
    else
    {
        //ошибка парсинга
        return false;
    } //Конец парсинга JSON
//Чтобы не терять контекст - все следующие действия тут же

//Создаем папки для web контента

//Создаю папку для нового контента
    string createContentFolder = "mkdir " + WEB_content + to_string(numFolderForWebContent);
    system(createContentFolder.c_str());

//Копирую актуальный zip файл
    string fileCurrentZIP = archiv_path_zip;
    fileCurrentZIP.append("/" + oboznachenieIkodDokumenta  + ".zip");
    string copyCurrentZIP = "cp " + fileCurrentZIP + " " + WEB_content + to_string(numFolderForWebContent);
    system(copyCurrentZIP.c_str());
//Переименовываю актуальный zip файл
    string renameCurrentZip = "mv " + WEB_content + to_string(numFolderForWebContent) + "/" + oboznachenieIkodDokumenta + ".zip " +
                                      WEB_content + to_string(numFolderForWebContent) + "/" + oboznachenieIkodDokumenta + ".изм" + changeNumStr + ".zip";
    system(renameCurrentZip.c_str());

//Копирую все файлы zip старых версий в текущую папку Имя файла oboznachenieIkodDokumenta Число файлов =   changeNum
    vector<pair<string,string>> oldZipData;
    for (auto i = 0; i < changeNum; ++i ) {
        string directoryOLD = archiv_path_zip;
        directoryOLD.append("/../");
        directoryOLD.append("Ниокр-Неактуальные_документы");
        string fileZIP = directoryOLD + "/" + oboznachenieIkodDokumenta + ".изм" + to_string(i) + ".zip";
        string copyZIP = "cp " + fileZIP + " " + WEB_content + to_string(numFolderForWebContent);
        system(copyZIP.c_str());
    //Разворачиваю zip
            string patchToFile = WEB_content + to_string(numFolderForWebContent);
            string fileName = oboznachenieIkodDokumenta + ".изм" + to_string(i) + ".zip";
            string patchToExtractDirectory = WEB_content + to_string(numFolderForWebContent) + "/" + oboznachenieIkodDokumenta + ".изм" + to_string(i);
            extractZip(patchToFile, fileName ,patchToExtractDirectory);
    //открываю JSON из папки zip
            string patchToZIPJSON = WEB_content + to_string(numFolderForWebContent)
                    + "/" + oboznachenieIkodDokumenta + ".изм" + to_string(i)
                    + "/" + oboznachenieIkodDokumenta + ".изм" + to_string(i) + ".zip/index.json";
            QFile fileZIPJSON(QString::fromStdString(patchToZIPJSON));
            fileZIPJSON.open(QIODevice::ReadOnly);
            QString jsonZIPData = fileZIPJSON.readAll();
            fileZIPJSON.close();
            Document documentZIP;
            documentZIP.Parse(jsonZIPData.toStdString().c_str());
            const   Value & requisitesZIP = documentZIP["Реквизиты документа по ГОСТ 2.104"];
            string changeNotificationNumZIP = requisitesZIP["Номер извещения об изменении"].GetString();
            string notificationDataStrZIP = requisitesZIP["Дата извещения об изменении"].GetString();
    //Сохраняю данные для карточки
            oldZipData.push_back(make_pair(changeNotificationNumZIP, notificationDataStrZIP));

    //Копирую  файл извещения об изменении из папки Ниокр-Извещения_об_изменении и переименовываю pdf в PDF
            string path_to_IZV = archiv_path_zip;
            path_to_IZV.append("/../");
            path_to_IZV.append("Ниокр-Извещения_об_изменении");
            string nIZM_old_PDF = path_to_IZV + "/" + changeNotificationNumZIP + ".PDF";
            string nIZM_old_pdf = path_to_IZV + "/" + changeNotificationNumZIP + ".pdf";
            string copyIZM_old_PDF = "cp " + nIZM_old_PDF + " " + WEB_content + to_string(numFolderForWebContent) + " 2> /dev/null";
            string copyIZM_old_pdf = "cp " + nIZM_old_pdf + " " + WEB_content + to_string(numFolderForWebContent) + " 2> /dev/null";
            system(copyIZM_old_PDF.c_str());
            system(copyIZM_old_pdf.c_str());
            string renameIZM_old_PDF = "mv " + WEB_content + to_string(numFolderForWebContent) + "/" + changeNotificationNumZIP + ".pdf " +
                    WEB_content + to_string(numFolderForWebContent) + "/" + changeNotificationNumZIP + ".PDF";// + " 2> /dev/null";
            system(renameIZM_old_PDF.c_str());
    }


//Формирую строку с контентом для QR и сам QR
    string stringForQr = createStringForQr (oboznachenieIkodDokumenta,
                                            changeNumStr,
                                            changeNotificationNum,
                                            storageDataStr,
                                            litera,
                                            controlSummOrigin,
                                            controlSummParts
                                            );
    createQR(oboznachenieIkodDokumenta, stringForQr);

//Создаю файл  rowContent - одна строка сайта
    vector <string> content;
    content.push_back(oboznachenieIkodDokumenta);
    content.push_back(naimenovanieIzdeliya);
    content.push_back(naimenovanieDokumenta);
    content.push_back(changeNumStr);
    content.push_back(notificationDataStr);
    content.push_back(controlSummOrigin);
    content.push_back(infoOrderList);

//Дальнейшие данные для карточки документа
    content.push_back("Подлинник");
    content.push_back(inventoryNumOriginal);
    content.push_back(storageDataStr);
    content.push_back(creater);
    content.push_back(crc32Main);
    content.push_back(crc32Contents);
    if (litera == "")
        content.push_back("нет");
    if (litera != "")
        content.push_back(litera);
    content.push_back(software);
    if (tags == "")
        content.push_back("нет");
    if (tags != "")
        content.push_back(tags);
    content.push_back(changeNotificationNum);
    content.push_back(notificationDataStr);

    QFile fout((WEB_content + to_string(numFolderForWebContent) + "/rowContent").c_str());
    fout.open(QIODevice::WriteOnly);
    QByteArray ba;
    for (int i = 0; i < 7; ++i){//берем из content только первые 7 значений
        ba.append(QString::fromStdString(content[i]));
        ba.append('\n');
    }
    fout.write(ba);
    fout.close();

//Копирую в папку для web-контента pdf файл и переименовываю pdf в PDF
   // cp опции /путь/к/файлу/источнику /путь/к/директории/назначения
    string nPDF = namePDF.toStdString();
    string npdf = namePDF.chopped(3).toStdString().append("pdf");
    string copyPDF = "cp " + nPDF + " " + WEB_content + to_string(numFolderForWebContent) + " 2> /dev/null";
    string copypdf = "cp " + npdf + " " + WEB_content + to_string(numFolderForWebContent) + " 2> /dev/null";
    system(copyPDF.c_str());
    system(copypdf.c_str());
    string renamePDF = "mv " + WEB_content + to_string(numFolderForWebContent) + "/" + oboznachenieIkodDokumenta + ".pdf " +
            WEB_content + to_string(numFolderForWebContent) + "/" + oboznachenieIkodDokumenta + ".PDF"+ " 2> /dev/null";
    system(renamePDF.c_str());

//Копирую  ИУЛ из папки Ниокр-Документы_по_обозначениям и переименовываю pdf в PDF
    string path_to_IUL = archiv_path_zip;
//    auto posBlank1 = path_to_IUL.find(" ");//Борьба с пробелом (Пути файлов уже не содержат пробелов)
//    path_to_IUL.insert(posBlank1,"\\");
    path_to_IUL.append("/../");
    path_to_IUL.append("Ниокр-Документы_по_обозначениям");
    string nIULPDF = path_to_IUL + "/" + infoOrderList + ".PDF";
    string nIULpdf = path_to_IUL + "/" + infoOrderList + ".pdf";
    string copyIULPDF = "cp " + nIULPDF + " " + WEB_content + to_string(numFolderForWebContent) + " 2> /dev/null";
    string copyIULpdf = "cp " + nIULpdf + " " + WEB_content + to_string(numFolderForWebContent) + " 2> /dev/null";
    system(copyIULPDF.c_str());
    system(copyIULpdf.c_str());
    string renameIULPDF = "mv " + WEB_content + to_string(numFolderForWebContent) + "/" + infoOrderList + ".pdf " +
            WEB_content + to_string(numFolderForWebContent) + "/" + infoOrderList + ".PDF" + " 2> /dev/null";
    system(renameIULPDF.c_str());

//Создаем HTML документ и сохраняем его в папке
    string html = createHTML(content, oldZipData);
    QFile fhtml((WEB_content + to_string(numFolderForWebContent) + "/document.html").c_str());
    fhtml.open(QIODevice::WriteOnly);
    fhtml.write(html.c_str());
    fhtml.close();

//Копирую  файл извещения об изменении из папки Ниокр-Извещения_об_изменении и переименовываю pdf в PDF
    string path_to_IZM = archiv_path_zip;
//    auto posBlank2 = path_to_IZM.find(" ");//Борьба с пробелом (Пути файлов уже не содержат пробелов)
//    path_to_IZM.insert(posBlank2,"\\");
    path_to_IZM.append("/../");
    path_to_IZM.append("Ниокр-Извещения_об_изменении");
    string nIZMPDF = path_to_IZM + "/" + changeNotificationNum + ".PDF";
    string nIZMpdf = path_to_IZM + "/" + changeNotificationNum + ".pdf";
    string copyIZMPDF = "cp " + nIZMPDF + " " + WEB_content + to_string(numFolderForWebContent) + " 2> /dev/null";
    string copyIZMpdf = "cp " + nIZMpdf + " " + WEB_content + to_string(numFolderForWebContent) + " 2> /dev/null";
    system(copyIZMPDF.c_str());
    system(copyIZMpdf.c_str());
    string renameIZMPDF = "mv " + WEB_content + to_string(numFolderForWebContent) + "/" + changeNotificationNum + ".pdf " +
            WEB_content + to_string(numFolderForWebContent) + "/" + changeNotificationNum + ".PDF" + " 2> /dev/null";
    system(renameIZMPDF.c_str());



//Все действия в контексте текущей папки веб контента завершены

    ++numFolderForWebContent;
    if (numContent == numFolderForWebContent)
    {
//Все папки с контентом сформированы - формирую файл numDoc
        QFile frow((WEB_content + "/" + "numDoc").c_str());
        frow.open(QIODevice::WriteOnly);
        frow.write(to_string(numFolderForWebContent).c_str());
        frow.close();

//Формирую строку с датой: Сводный перечень документов на хранении по состоянию на ____
        QFile fdate((WEB_content + "/" + "date").c_str());
        fdate.open(QIODevice::WriteOnly);
        string date = "Сводный перечень документов на хранении по состоянию на ";
        date.append(currentDateTime());
        fdate.write(date.c_str());
        fdate.close();
    }
return true;
}



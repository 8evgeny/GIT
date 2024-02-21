#include "main.h"
extern string WEB_content;
extern uint numZipFiles;
extern uint numFolderForWebContent;
QString namePDF;
bool namePDFlettersBig = true;
extern bool printDebug;
extern int errorParsingJson;
extern string nameFileForDeleteIfJsonNoGood;
extern connection* ConnectionToDB;
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
                    "(КОМПАС.*)|"
                    "(Altium Designer 20.0.12 /(Build 288/))|"
                    "(Libre Office Writer 7.5.3.2)|"
                    "(Inkscape 1.2)|"
                    "(NanoCAD 23.0.6169.4115 /(Сборка 6276/))|"
                    "(Altium Designer.*)|"
                    "(Microsoft Word.*)"
                    "$");
    QRegExp iulStr("^"
                   "(ГРЛМ.[0-9]{6}.[0-9]{3}[А-Я][А-Я0-9]+)-УЛ|"
                   "(ГРЛМ.[0-9]{6}.[0-9]{3}УЛ)|"
                   "((ГРЛМ.[0-9]{6}.[0-9]{3}-[0-9]{2,3}[А-Я][А-Я0-9]+)-УЛ)|"
                   "$");
    Document document;
    if (!document.Parse(jsonData.toStdString().c_str()).HasParseError())
    {
        if (document.IsObject())
        {
            if (document.HasMember("Реквизиты документа по ГОСТ 2.104"))
            {
#ifdef printJson
                cout << "   Реквизиты документа по ГОСТ 2.104:\n";
#endif
                const   Value & requisites = document["Реквизиты документа по ГОСТ 2.104"];
                if (requisites.HasMember("Наименование изделия")){
                    naimenovanieIzdeliya = requisites["Наименование изделия"].GetString();
                    if(blankStr.exactMatch(QString::fromStdString(naimenovanieIzdeliya))) return false;
#ifdef printJson
                    cout << "\tНаименование изделия: " << naimenovanieIzdeliya << "\n";
#endif
                }else return false;
                if (requisites.HasMember("Наименование документа")){
                    naimenovanieDokumenta = requisites["Наименование документа"].GetString();
                    if(blankStr.exactMatch(QString::fromStdString(naimenovanieDokumenta))) return false;
#ifdef printJson
                    cout << "\tНаименование документа: " << naimenovanieDokumenta << "\n";
#endif
                }else return false;
                if (requisites.HasMember("Обозначение и код документа")){
                    oboznachenieIkodDokumenta = requisites["Обозначение и код документа"].GetString();
                    nameFileForDeleteIfJsonNoGood = oboznachenieIkodDokumenta;
                    if(blankStr.exactMatch(QString::fromStdString(oboznachenieIkodDokumenta))) return false;
      //вычисляем CRC32 PDF файла
      string appPdf = oboznachenieIkodDokumenta;
      string chopped = patchToFile;
      namePDF = QString::fromStdString(chopped).chopped(10)+QString("Contents/")+QString::fromStdString(appPdf.append(".PDF"));
#ifdef printJson
      QString directoryPDF = QString::fromStdString(chopped).chopped(10)+QString("Contents/");
      string printDir = "ls -l " + directoryPDF.toStdString();
      system(printDir.c_str());
#endif
      crc32 = CRC32(namePDF);
#ifdef printJson
      cout << "\tОбозначение и код документа: " << oboznachenieIkodDokumenta ;
#endif
      if(crc32 == -1)
      {
          printf("                      \tОтсутствует PDF файл  %s.PDF !!!\n", oboznachenieIkodDokumenta.c_str());
          return false;
      }
#ifdef printJson
      printf("                      \t(посчитан CRC32: %.8X)\n", crc32);
#endif
      char buf[20];
      sprintf(buf,"%.8X\n", crc32);
      string tmp{buf};
      crc32Main = tmp;

          //вычисляем CRC32 папки Contents
      string chopped1 = patchToFile;
      chopped1.insert(11,"\""); //Борьба с пробелом имени
      QString nameDirectory = QString::fromStdString(chopped1).chopped(11)+"\""+QString("/Contents");
      crc32Contents = CRC32Contents(nameDirectory);
#ifdef printJson
      printf("                      \t\t\t\t\t\t\t\t(посчитан CRC32 папки Contents: %s)\n", crc32Contents.c_str());
#endif
                }else return false;
                if (requisites.HasMember("Общее количество листов документа")){
                    auto type = requisites["Общее количество листов документа"].GetType();
                    if (type == rapidjson::kStringType)
                       numberSheets =  atoi(requisites["Общее количество листов документа"].GetString());
                    if (type == rapidjson::kNumberType)
                       numberSheets =  requisites["Общее количество листов документа"].GetInt();
                    if(!number3dig.exactMatch(QString::number(numberSheets))) return false;
#ifdef printJson
                    cout << "\tОбщее количество листов документа: " << numberSheets << "\n";
#endif
                }else return false;
                if (requisites.HasMember("Наименование или код организации")){
                    company = requisites["Наименование или код организации"].GetString();
                    if(company != "ООО «Группа индустриальных технологий»") return false;
#ifdef printJson
                    cout << "\tНаименование или код организации: " << company << "\n";
#endif
                }else return false;
                if (requisites.HasMember("Сведения о подписании документа")){
#ifdef printJson
                    cout << "\tСведения о подписании документа:\n";
#endif
                    const   Value & infoAboutSigning = requisites["Сведения о подписании документа"];
                    if (infoAboutSigning.HasMember("Разработал")){
                        creater = infoAboutSigning["Разработал"].GetString();
                        if(blankStr.exactMatch(QString::fromStdString(creater))) return false;
#ifdef printJson
                        cout << "\t\tРазработал: " << creater << "\n";
#endif
                    }else return false;
                    if (infoAboutSigning.HasMember("Дата")){
                        createdDataStr = infoAboutSigning["Дата"].GetString();
                        if(!dataStr.exactMatch(QString::fromStdString(createdDataStr))) return false;
                        stringstream ss(createdDataStr);
                        tm tm = {};
                        ss >> get_time(&tm, "%Y-%m-%d");
                        createdData = chrono::system_clock::from_time_t(mktime(&tm));
#ifdef printJson
                        cout << "\t\tДата: " << createdData << "\n";
#endif
                    }else return false;
                    if (infoAboutSigning.HasMember("Информационно-удостоверяющий лист")){
                        infoOrderList = infoAboutSigning["Информационно-удостоверяющий лист"].GetString();
//                        if(!iulStr.exactMatch(QString::fromStdString(infoOrderList))) return false;
#ifdef printJson
                        cout << "\t\tИнформационно-удостоверяющий лист: " << infoOrderList << "\n";
#endif
                    }else return false;
                }else return false;
                if (requisites.HasMember("Номер изменения")){
                    auto type = requisites["Номер изменения"].GetType();
                    if (type == rapidjson::kStringType)
                        changeNum =  atoi(requisites["Номер изменения"].GetString());
                    if (type == rapidjson::kNumberType)
                        changeNum =  requisites["Номер изменения"].GetInt();
                        if(!number2dig.exactMatch(QString::number(changeNum))) return false;
#ifdef printJson
                    cout << "\tНомер изменения: " << changeNum << "\n";
#endif
                    changeNumStr = to_string(changeNum);
                }else return false;
                if (requisites.HasMember("Номер извещения об изменении")){
                    changeNotificationNum = requisites["Номер извещения об изменении"].GetString();
                    if(blankStr.exactMatch(QString::fromStdString(changeNotificationNum))) return false;
#ifdef printJson
                    cout << "\tНомер извещения об изменении: " << changeNotificationNum << "\n";
#endif
                }else return false;
                if (requisites.HasMember("Дата извещения об изменении")){
                    notificationDataStr = requisites["Дата извещения об изменении"].GetString();
                    if(!dataStr.exactMatch(QString::fromStdString(notificationDataStr))) return false;
                    stringstream ss(notificationDataStr);
                    tm tm = {};
                    ss >> get_time(&tm, "%Y-%m-%d");
                    notificationData = chrono::system_clock::from_time_t(mktime(&tm));
#ifdef printJson
                    cout << "\tДата извещения об изменении: " << notificationData << "\n";
#endif
                }else return false;
                if (requisites.HasMember("Инвентарный номер подлинника")){
                    inventoryNumOriginal = requisites["Инвентарный номер подлинника"].GetString();
                    if(blankStr.exactMatch(QString::fromStdString(inventoryNumOriginal))) return false;
#ifdef printJson
                    cout << "\tИнвентарный номер подлинника: " << inventoryNumOriginal << "\n";
#endif
                }else return false;
                if (requisites.HasMember("Дата приемки на хранение")){
                    storageDataStr = requisites["Дата приемки на хранение"].GetString();
                    if(!dataStr.exactMatch(QString::fromStdString(storageDataStr))) return false;
                    stringstream ss(storageDataStr);
                    tm tm = {};
                    ss >> get_time(&tm, "%Y-%m-%d");
                    storageData = chrono::system_clock::from_time_t(mktime(&tm));
#ifdef printJson
                    cout << "\tДата приемки на хранение: " << storageData << "\n";
#endif
                }else return false;
                if (requisites.HasMember("Литера")){
                    litera = requisites["Литера"].GetString();
                             if (litera == ""){
#ifdef printJson
                        cout << "\tЛитера: " << "\"\"" << "\n";
#endif
                             }
                             else {
#ifdef printJson
                        cout << "\tЛитера: " << litera << "\n";
#endif
                             }
                }else return false;
                if (requisites.HasMember("Код документа в зависимости от характера использования")){
                    auto type = requisites["Код документа в зависимости от характера использования"].GetType();
                    if (type == rapidjson::kStringType)
                        dokumentCode =  atoi(requisites["Код документа в зависимости от характера использования"].GetString());
                    if (type == rapidjson::kNumberType)
                        dokumentCode =  requisites["Код документа в зависимости от характера использования"].GetInt();
                    if(!number2dig.exactMatch(QString::number(dokumentCode))) return false;
#ifdef printJson
                    cout << "\tКод документа в зависимости от характера использования: " << dokumentCode << "\n";
#endif
                }
            }//Реквизиты документа
            else return false;

            if (document.HasMember("Сервисные данные")){
#ifdef printJson
                cout << "   Сервисные данные:\n";
#endif
                const   Value & serviceData = document["Сервисные данные"];
                if (serviceData.HasMember("Версия формата реквизитной части")){
                    auto type = serviceData["Версия формата реквизитной части"].GetType();
                    if (type == rapidjson::kStringType)
                        version =  atoi(serviceData["Версия формата реквизитной части"].GetString());
                    if (type == rapidjson::kNumberType)
                        version =  serviceData["Версия формата реквизитной части"].GetInt();
                    if(!number2dig.exactMatch(QString::number(version))) return false;
#ifdef printJson
                    cout << "\tВерсия формата реквизитной части: " << version << "\n";
#endif
                }else return false;
                if (serviceData.HasMember("Алгоритм расчета контрольной суммы")){
                    algorithm = serviceData["Алгоритм расчета контрольной суммы"].GetString();
                    if(blankStr.exactMatch(QString::fromStdString(algorithm))) return false;
#ifdef printJson
                    cout << "\tАлгоритм расчета контрольной суммы: " << algorithm << "\n";
#endif
                }else return false;
                if (serviceData.HasMember("Значение контрольной суммы подлинника")){
                    controlSummOrigin = serviceData["Значение контрольной суммы подлинника"].GetString();
                if(!crc32Str.exactMatch(QString::fromStdString(controlSummOrigin))) return false;

                    //Сравниваем с расчитанным CRC32
                    char buf[20];
                    sprintf (buf,"%.8X", crc32);
                    string calculateCRC32{buf};
#ifdef printJson
                    cout << "\tЗначение контрольной суммы подлинника: " << controlSummOrigin ;
#endif
                    if(controlSummOrigin == calculateCRC32) {
#ifdef printJson
                        cout << "                  \t(Контрольные суммы совпадают)"<<endl;
#endif
                    }
                    else {
#ifdef printJson
                        cout << "                  \t(Контрольные суммы не совпадают !!!)"<<endl;
#endif
                        return false;}

                }else return false;
                if (serviceData.HasMember("Значение контрольной суммы содержательных частей")){
                controlSummParts = serviceData["Значение контрольной суммы содержательных частей"].GetString();
                if(!crc32Str.exactMatch(QString::fromStdString(controlSummParts))) return false;
#ifdef printJson
                cout << "\tЗначение контрольной суммы содержательных частей: " << controlSummParts ;
#endif
                if(controlSummParts == crc32Contents) {
#ifdef printJson
                    cout << "  \t\t(Контрольные суммы содержательных частей совпадают)"<<endl;
#endif
                }
                else {
#ifdef printJson
                    cout << "  \t\t(Контрольные суммы содержательных частей не совпадают !!!)"<<endl;
#endif
                    return false;}

                }else return false;
                if (serviceData.HasMember("Программное обеспечение для редактирования исходных данных")){
                    software = serviceData["Программное обеспечение для редактирования исходных данных"].GetString();
                    if(!softStr.exactMatch(QString::fromStdString(software))) return false;
#ifdef printJson
                    cout << "\tПрограммное обеспечение для редактирования исходных данных: " << software << "\n";
#endif
                }else return false;
                if (serviceData.HasMember("Объем в листах А4")){
                    auto type = serviceData["Объем в листах А4"].GetType();
                    if (type == rapidjson::kStringType)
                        volumeInSheets =  atoi(serviceData["Объем в листах А4"].GetString());
                    if (type == rapidjson::kNumberType)
                        volumeInSheets =  serviceData["Объем в листах А4"].GetInt();
                    if(!number3dig.exactMatch(QString::number(volumeInSheets))) return false;
#ifdef printJson
                    cout << "\tОбъем в листах А4: " << volumeInSheets << "\n";
#endif

                }else return false;
                if (serviceData.HasMember("Документ действителен")){
                    auto type = serviceData["Документ действителен"].GetType();
                    if (type == rapidjson::kTrueType || rapidjson::kFalseType)
                       isValid = serviceData["Документ действителен"].GetBool();
                    else return false;
#ifdef printJson
                    cout << "\tДокумент действителен: " << boolalpha << isValid << "\n";
#endif
                }else return false;
                if (serviceData.HasMember("Теги")){
                    tags = serviceData["Теги"].GetString();
                           if (tags == ""){
#ifdef printJson
                        cout << "\tТеги: " << "\"\"" << "\n";
#endif
                           }
                           else {
#ifdef printJson
                        cout << "\tТеги: " << tags << "\n\n";
#endif
                           }
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

//1. Создаю папку для нового контента
if (printDebug) cout<<"1"<<endl;
    string createContentFolder = "mkdir " + WEB_content + to_string(numFolderForWebContent);
    cout << "Create folder mumber " << to_string(numFolderForWebContent) << endl;
    system(createContentFolder.c_str());

//2. Копирую актуальный zip файл
if (printDebug) cout<<"2"<<endl;
    string fileCurrentZIP = archiv_path_zip;
    fileCurrentZIP.append("/" + oboznachenieIkodDokumenta  + ".zip");
    string tmp1 = fileCurrentZIP;
    auto pos1 = tmp1.find_last_of("/");
    tmp1.insert(pos1 + 1,"\"");
    tmp1.append("\"");//Борьба с пробелом в имени
    string copyCurrentZIP = "cp " + tmp1 + " " + WEB_content + to_string(numFolderForWebContent);
    system(copyCurrentZIP.c_str());

//3. Переименовываю актуальный zip файл
if (printDebug) cout<<"3"<<endl;
    string tmp2 = oboznachenieIkodDokumenta + ".zip";
    tmp2 = addDoubleQuotesToName(tmp2);
    string tmp3 = oboznachenieIkodDokumenta + ".изм" + changeNumStr + ".zip";
    tmp3 = addDoubleQuotesToName(tmp3);
    string renameCurrentZip = "mv " + WEB_content + to_string(numFolderForWebContent) + "/" + tmp2 + " " +
                                      WEB_content + to_string(numFolderForWebContent) + "/" + tmp3;
    system(renameCurrentZip.c_str());

//*******************************************************************
//Сделать для изм > 0 с пробелом в имени !!
//*******************************************************************
//4. Копирую все файлы zip старых версий в текущую папку Имя файла oboznachenieIkodDokumenta Число файлов =   changeNum
if (printDebug) cout<<"4"<<endl;
    vector<pair<string,string>> oldZipData;
    for (auto i = 0; i < changeNum; ++i ) {
        string directoryOLD = archiv_path_zip;
        directoryOLD.append("/../");
        directoryOLD.append("NIOKR_old_DOC");
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

    //Копирую  файл извещения об изменении из папки NIOKR_messages и переименовываю pdf в PDF
            string path_to_IZV = archiv_path_zip;
            path_to_IZV.append("/../");
            path_to_IZV.append("NIOKR_messages");
            string nIZM_old_PDF = path_to_IZV + "/" + changeNotificationNumZIP + ".PDF";
            string nIZM_old_pdf = path_to_IZV + "/" + changeNotificationNumZIP + ".pdf";
            string copyIZM_old_PDF = "cp " + nIZM_old_PDF + " " + WEB_content + to_string(numFolderForWebContent);
            string copyIZM_old_pdf = "cp " + nIZM_old_pdf + " " + WEB_content + to_string(numFolderForWebContent);
            system(copyIZM_old_PDF.c_str());
            system(copyIZM_old_pdf.c_str());
            string renameIZM_old_PDF = "mv " + WEB_content + to_string(numFolderForWebContent) + "/" + changeNotificationNumZIP + ".pdf " +
                    WEB_content + to_string(numFolderForWebContent) + "/" + changeNotificationNumZIP + ".PDF";
            system(renameIZM_old_PDF.c_str());
    }
//*******************************************************************
//Сделать для изм > 0 с пробелом в имени !!
//*******************************************************************


//5. Формирую строку с контентом для QR и сам QR
if (printDebug) cout<<"5"<<endl;
    string stringForQr = createStringForQr (oboznachenieIkodDokumenta,
                                            changeNumStr,
                                            changeNotificationNum,
                                            storageDataStr,
                                            litera,
                                            controlSummOrigin,
                                            controlSummParts
                                            );
    createQR(oboznachenieIkodDokumenta, stringForQr);

//6. Создаю файл  rowContent - одна строка сайта
if (printDebug) cout<<"6"<<endl;
    vector <string> content;
    content.push_back(oboznachenieIkodDokumenta);
    content.push_back(naimenovanieIzdeliya);
    content.push_back(naimenovanieDokumenta);
    content.push_back(changeNumStr);
    content.push_back(notificationDataStr);
    content.push_back(controlSummOrigin);
    content.push_back(infoOrderList);

//Заполняем таблицу
    string sql = "INSERT INTO WEB_CONTENT ("
                 "folder,"
                 "oboznachenieIkodDokumenta,"
                 "naimenovanieIzdeliya,"
                 "naimenovanieDokumenta,"
                 "changeNumStr,"
                 "notificationDataStr,"
                 "controlSummOrigin,"
                 "infoOrderList) "
          "VALUES ('"
                 + to_string(numFolderForWebContent)
        + "','" + oboznachenieIkodDokumenta
        + "','" + naimenovanieIzdeliya
        + "','" + naimenovanieDokumenta
        + "','" + changeNumStr
        + "','" + notificationDataStr
        + "','" + controlSummOrigin
        + "','" + infoOrderList
        + "');";
    try {
    transactionToDB(ConnectionToDB, sql);
    } catch (const std::exception &e) {
        cerr << e.what() << std::endl;
    }

//7. Дальнейшие данные для карточки документа
if (printDebug) cout<<"7"<<endl;
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

//8. Копирую в папку для web-контента pdf файл и переименовываю pdf в PDF
if (printDebug) cout<<"8"<<endl;
   // cp опции /путь/к/файлу/источнику /путь/к/директории/назначения
    string tmp4 = "\"" + oboznachenieIkodDokumenta ;
    string tmp5 = patchToFile;
    QString tmp6;
    if (namePDF.toStdString().ends_with("pdf")){
        tmp6 = QString::fromStdString(tmp5).chopped(11)+"\""+QString("/Contents/")+QString::fromStdString(tmp4.append(".pdf\""));
    }
    if (namePDF.toStdString().ends_with("PDF")){
        tmp6 = QString::fromStdString(tmp5).chopped(11)+"\""+QString("/Contents/")+QString::fromStdString(tmp4.append(".PDF\""));
    }
    string tmp7 = tmp6.toStdString();
    tmp7.insert(11,"\"");
    string copyPDF = "cp " + tmp7 + " " + WEB_content + to_string(numFolderForWebContent);
    system(copyPDF.c_str());

    if (namePDF.toStdString().ends_with("pdf")){
        string rename = "mv " + WEB_content + to_string(numFolderForWebContent) + "/\"" + oboznachenieIkodDokumenta + ".pdf\" " +
            WEB_content + to_string(numFolderForWebContent) + "/\"" + oboznachenieIkodDokumenta + ".PDF\"";
        system(rename.c_str());
    }

//9. Копирую  ИУЛ из папки NIOKR_IUL и переименовываю pdf в PDF
if (printDebug) cout<<"9"<<endl;
    string path_to_IUL = archiv_path_zip;
//    auto posBlank1 = path_to_IUL.find(" ");//Борьба с пробелом (Пути файлов уже не содержат пробелов)
//    path_to_IUL.insert(posBlank1,"\\");
    path_to_IUL.append("/../");
    path_to_IUL.append("NIOKR_IUL");
    string nameIulPDF;
    if (namePDFlettersBig){
        nameIulPDF = path_to_IUL + "/" + infoOrderList + ".PDF";
    }
    else{
        nameIulPDF = path_to_IUL + "/\"" + infoOrderList + ".pdf\"";
    }
    string copyIulPDF = "cp " + nameIulPDF + " " + WEB_content + to_string(numFolderForWebContent);
    system(copyIulPDF.c_str());
    if (!namePDFlettersBig){
        string renameIULPDF = "mv " + WEB_content + to_string(numFolderForWebContent) + "/\"" + infoOrderList + ".pdf\" " +
                WEB_content + to_string(numFolderForWebContent) + "/\"" + infoOrderList + ".PDF\"";
        system(renameIULPDF.c_str());
    }

//10. Создаем HTML документ и сохраняем его в папке
if (printDebug) cout<<"10"<<endl;
    string html = createHTML(content, oldZipData);
    QFile fhtml((WEB_content + to_string(numFolderForWebContent) + "/document.html").c_str());
    fhtml.open(QIODevice::WriteOnly);
    fhtml.write(html.c_str());
    fhtml.close();

//11. Копирую  файл извещения об изменении из папки NIOKR_messages и переименовываю pdf в PDF
if (printDebug) cout<<"11"<<endl;
    string path_to_IZM = archiv_path_zip;
//    auto posBlank2 = path_to_IZM.find(" ");//Борьба с пробелом (Пути файлов уже не содержат пробелов)
//    path_to_IZM.insert(posBlank2,"\\");
    path_to_IZM.append("/../");
    path_to_IZM.append("NIOKR_messages");
    string nIZMPDF;
    if (namePDFlettersBig){
        nIZMPDF = path_to_IZM + "/" + changeNotificationNum + ".PDF";
    }
    else{
        nIZMPDF = path_to_IZM + "/" + changeNotificationNum + ".pdf";
    }
    string copyIZMPDF = "cp " + nIZMPDF + " " + WEB_content + to_string(numFolderForWebContent);
    system(copyIZMPDF.c_str());
    if (!namePDFlettersBig){
        string renameIZMPDF = "mv " + WEB_content + to_string(numFolderForWebContent) + "/" + changeNotificationNum + ".pdf " +
                WEB_content + to_string(numFolderForWebContent) + "/" + changeNotificationNum + ".PDF";
        system(renameIZMPDF.c_str());
    }
    cout << "Copy content to folder " << to_string(numFolderForWebContent) << endl;

//Все действия в контексте текущей папки веб контента завершены
    ++numFolderForWebContent;

//Тут можно удалить исходную папку откуда копировали
    string remove = "rm -rf ../EXTRACT/" + oboznachenieIkodDokumenta;
    system(remove.c_str());

return true;
}

string addDoubleQuotesToName(string & name){
    string tmp = name;
    auto pos = tmp.find_last_of("/");
    tmp.insert(pos + 1,"\"");
    tmp += "\"";
    return tmp;
}

#include "main.h"
extern string WEB_content;
uint numFolderForWebContent{1};
ostream& operator<<(ostream &os, const chrono::time_point<chrono::system_clock> &t){
    const auto tt   (chrono::system_clock::to_time_t(t));
    const auto loct (localtime(&tt));
    return os << put_time(loct, "%Y-%m-%d");//%c
}
using days = chrono::duration<chrono::hours::rep, ratio_multiply<chrono::hours::period, ratio<24>>>;
constexpr days operator ""_days(unsigned long long d){
    return days{d};
}

void createQR(string nameQR, string originText)
{
#ifdef WIN32
    string CMD1 = "qrcodecon -s 6 -l M -o ";
    string CMD2 = nameQR+".png";
#else
    string CMD1 = "qrencode -s 6 -l M -o ";
    string CMD2 = WEB_content + to_string(numFolderForWebContent) + "/" + nameQR+".png";
#endif
    string CMD3 = originText;
    string CMD = CMD1 +  CMD2 + " " + CMD3;
    system (CMD.c_str());
}


string createStringForQr (string oboznachenieIkodDokumenta,
                          string changeNumStr,
                          string changeNotificationNum,
                          string storageDataStr,
                          string litera,
                          string controlSummOrigin,
                          string controlSummParts
                          )
{
//    ГРЛМ.741138.003 | изм.0 | К.001-2023 | 2023-05-29 | Лит.нет | CRC-32.pdf=1F349207 | CRC-32.zip\Contents=3C3DE8F8

    string separator = "\" | \"";
    string LiteraQr{"Лит."};
    if (litera == "")
        LiteraQr.append("нет");
    else
        LiteraQr.append(litera);

    string result = oboznachenieIkodDokumenta + separator +
            "изм."+changeNumStr + separator +
            changeNotificationNum + separator +
            storageDataStr  + separator +
            LiteraQr + separator+
            "CRC-32.pdf="+controlSummOrigin + separator +
            "CRC-32.zip\\Contents=" + controlSummParts;
//    cout<<result<<endl;
    return result;
}

bool parseJSON(string & patchToFile){
    QFile file(QString::fromStdString(patchToFile));
    file.open(QIODevice::ReadOnly);
    QString jsonData = file.readAll();
    file.close();

    //Переменные
    string naimenovanieIzdeliya{""};
    string naimenovanieDokumenta{""};
    string oboznachenieIkodDokumenta{""};                     //1
    quint32 crc32; //СКС32 PDF файла
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
                "(Altium Designer 20.0.12 (Build 288))|"
                "(Libre Office Writer 7.5.3.2)|"
                "(Inkscape 1.2)|"
                "(NanoCAD 23.0.6169.4115 (Сборка 6276))"
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
      QString namePDF = QString::fromStdString(chopped).chopped(10)+QString("Contents/")+QString::fromStdString(appPdf.append(".PDF"));
      crc32 = CRC32(namePDF);
      cout << "\tОбозначение и код документа: " << oboznachenieIkodDokumenta ;
      if(crc32 == -1)
      {
          printf("                      \tОтсутствует PDF файл  %s.PDF !!!\n", oboznachenieIkodDokumenta.c_str());
          return false;
      }
      printf("                      \t(посчитан CRC32: %.8X)\n", crc32);


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
    }
//Создаем папки для web контента

    // создаем папку для нового контента
    string createContentFolder = "mkdir " + WEB_content + to_string(numFolderForWebContent);
    system(createContentFolder.c_str());

string stringForQr = createStringForQr (oboznachenieIkodDokumenta,
                                        changeNumStr,
                                        changeNotificationNum,
                                        storageDataStr,
                                        litera,
                                        controlSummOrigin,
                                        controlSummParts
                                        );

createQR(oboznachenieIkodDokumenta, stringForQr);

//Формирую строку с контентом для сайта
vector <string> content;
content.push_back(oboznachenieIkodDokumenta);
content.push_back(naimenovanieIzdeliya);
content.push_back(naimenovanieDokumenta);
content.push_back(changeNumStr);
content.push_back(notificationDataStr);
content.push_back(controlSummOrigin);
content.push_back(infoOrderList);

QFile fout((WEB_content + to_string(numFolderForWebContent) + "/" + to_string(numFolderForWebContent)).c_str());
fout.open(QIODevice::WriteOnly);
QByteArray ba;
for (int i = 0; i<content.size(); ++i)
{
    ba.append(QString::fromStdString(content[i]));
    ba.append('\n');
}
fout.write(ba);
fout.close();
++numFolderForWebContent;
return true;
}


bool extractZip(string & patchToFile, string & fileName, string & patchToExtractDirectory)
{
// 7z x /home/evg/SOFT/Github/GIT/16_WEB_ARHIV/Ниокр-Актуальные\ документы/ГРЛМ.713121.001.zip -y -o"/home/evg/SOFT/Github/GIT/16_WEB_ARHIV/EXTRACT/ГРЛМ.713121.001"
    string createExtractFolder = "mkdir " + patchToExtractDirectory + "/" + fileName;
    system(createExtractFolder.c_str());
    string extract = "7z x " + patchToFile + " -y -o\"" + patchToExtractDirectory + "/" + fileName+ "\"";
    system(extract.c_str());
    return true;
}

const quint32 CRC32Table[256] =
{
    0x00000000, 0x77073096, 0xee0e612c, 0x990951ba,
    0x076dc419, 0x706af48f, 0xe963a535, 0x9e6495a3,
    0x0edb8832, 0x79dcb8a4, 0xe0d5e91e, 0x97d2d988,
    0x09b64c2b, 0x7eb17cbd, 0xe7b82d07, 0x90bf1d91,
    0x1db71064, 0x6ab020f2, 0xf3b97148, 0x84be41de,
    0x1adad47d, 0x6ddde4eb, 0xf4d4b551, 0x83d385c7,
    0x136c9856, 0x646ba8c0, 0xfd62f97a, 0x8a65c9ec,
    0x14015c4f, 0x63066cd9, 0xfa0f3d63, 0x8d080df5,
    0x3b6e20c8, 0x4c69105e, 0xd56041e4, 0xa2677172,
    0x3c03e4d1, 0x4b04d447, 0xd20d85fd, 0xa50ab56b,
    0x35b5a8fa, 0x42b2986c, 0xdbbbc9d6, 0xacbcf940,
    0x32d86ce3, 0x45df5c75, 0xdcd60dcf, 0xabd13d59,
    0x26d930ac, 0x51de003a, 0xc8d75180, 0xbfd06116,
    0x21b4f4b5, 0x56b3c423, 0xcfba9599, 0xb8bda50f,
    0x2802b89e, 0x5f058808, 0xc60cd9b2, 0xb10be924,
    0x2f6f7c87, 0x58684c11, 0xc1611dab, 0xb6662d3d,
    0x76dc4190, 0x01db7106, 0x98d220bc, 0xefd5102a,
    0x71b18589, 0x06b6b51f, 0x9fbfe4a5, 0xe8b8d433,
    0x7807c9a2, 0x0f00f934, 0x9609a88e, 0xe10e9818,
    0x7f6a0dbb, 0x086d3d2d, 0x91646c97, 0xe6635c01,
    0x6b6b51f4, 0x1c6c6162, 0x856530d8, 0xf262004e,
    0x6c0695ed, 0x1b01a57b, 0x8208f4c1, 0xf50fc457,
    0x65b0d9c6, 0x12b7e950, 0x8bbeb8ea, 0xfcb9887c,
    0x62dd1ddf, 0x15da2d49, 0x8cd37cf3, 0xfbd44c65,
    0x4db26158, 0x3ab551ce, 0xa3bc0074, 0xd4bb30e2,
    0x4adfa541, 0x3dd895d7, 0xa4d1c46d, 0xd3d6f4fb,
    0x4369e96a, 0x346ed9fc, 0xad678846, 0xda60b8d0,
    0x44042d73, 0x33031de5, 0xaa0a4c5f, 0xdd0d7cc9,
    0x5005713c, 0x270241aa, 0xbe0b1010, 0xc90c2086,
    0x5768b525, 0x206f85b3, 0xb966d409, 0xce61e49f,
    0x5edef90e, 0x29d9c998, 0xb0d09822, 0xc7d7a8b4,
    0x59b33d17, 0x2eb40d81, 0xb7bd5c3b, 0xc0ba6cad,
    0xedb88320, 0x9abfb3b6, 0x03b6e20c, 0x74b1d29a,
    0xead54739, 0x9dd277af, 0x04db2615, 0x73dc1683,
    0xe3630b12, 0x94643b84, 0x0d6d6a3e, 0x7a6a5aa8,
    0xe40ecf0b, 0x9309ff9d, 0x0a00ae27, 0x7d079eb1,
    0xf00f9344, 0x8708a3d2, 0x1e01f268, 0x6906c2fe,
    0xf762575d, 0x806567cb, 0x196c3671, 0x6e6b06e7,
    0xfed41b76, 0x89d32be0, 0x10da7a5a, 0x67dd4acc,
    0xf9b9df6f, 0x8ebeeff9, 0x17b7be43, 0x60b08ed5,
    0xd6d6a3e8, 0xa1d1937e, 0x38d8c2c4, 0x4fdff252,
    0xd1bb67f1, 0xa6bc5767, 0x3fb506dd, 0x48b2364b,
    0xd80d2bda, 0xaf0a1b4c, 0x36034af6, 0x41047a60,
    0xdf60efc3, 0xa867df55, 0x316e8eef, 0x4669be79,
    0xcb61b38c, 0xbc66831a, 0x256fd2a0, 0x5268e236,
    0xcc0c7795, 0xbb0b4703, 0x220216b9, 0x5505262f,
    0xc5ba3bbe, 0xb2bd0b28, 0x2bb45a92, 0x5cb36a04,
    0xc2d7ffa7, 0xb5d0cf31, 0x2cd99e8b, 0x5bdeae1d,
    0x9b64c2b0, 0xec63f226, 0x756aa39c, 0x026d930a,
    0x9c0906a9, 0xeb0e363f, 0x72076785, 0x05005713,
    0x95bf4a82, 0xe2b87a14, 0x7bb12bae, 0x0cb61b38,
    0x92d28e9b, 0xe5d5be0d, 0x7cdcefb7, 0x0bdbdf21,
    0x86d3d2d4, 0xf1d4e242, 0x68ddb3f8, 0x1fda836e,
    0x81be16cd, 0xf6b9265b, 0x6fb077e1, 0x18b74777,
    0x88085ae6, 0xff0f6a70, 0x66063bca, 0x11010b5c,
    0x8f659eff, 0xf862ae69, 0x616bffd3, 0x166ccf45,
    0xa00ae278, 0xd70dd2ee, 0x4e048354, 0x3903b3c2,
    0xa7672661, 0xd06016f7, 0x4969474d, 0x3e6e77db,
    0xaed16a4a, 0xd9d65adc, 0x40df0b66, 0x37d83bf0,
    0xa9bcae53, 0xdebb9ec5, 0x47b2cf7f, 0x30b5ffe9,
    0xbdbdf21c, 0xcabac28a, 0x53b39330, 0x24b4a3a6,
    0xbad03605, 0xcdd70693, 0x54de5729, 0x23d967bf,
    0xb3667a2e, 0xc4614ab8, 0x5d681b02, 0x2a6f2b94,
    0xb40bbe37, 0xc30c8ea1, 0x5a05df1b, 0x2d02ef8d
};

quint32 CRC32(QString fileName)
{
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly)){
       if(fileName.endsWith("PDF")){
          fileName.chop(3);
          fileName.append("pdf");
          file.setFileName(fileName);
          if(!file.open(QIODevice::ReadOnly))
              return -1;
       }
    }
    quint32 CRC32 = 0xffffffff;
    qint64 n, i;
    char *buf = new char [BUFSIZE];
    while((n = file.read(buf, BUFSIZE)) > 0){
        for (i = 0; i < n; i++)
            CRC32 = (CRC32 >> 8) ^ CRC32Table[(CRC32 ^ buf[i]) & 0xff];
    }
    CRC32 ^= 0xffffffff;
    delete[] buf;
    file.close();
    return CRC32;
}

string CRC32Contents(QString DirectoryPatch){

#ifdef WIN32
    char buf1[BUFSIZ*20]; //160кб
    std::size_t bufsize = BUFSIZ*20;

    int e = getenv_s(&bufsize,buf1,bufsize,"PATH");
    if (e) {
        std::cerr << "`getenv_s` failed, returned " << e << '\n';
        exit(EXIT_FAILURE);
    }
    std::string env_path = buf1;
//    std::cout << "In main process, `PATH`=" << env_path << std::endl;
    env_path += ";C:\\Program Files\\7-Zip";
    e = _putenv_s("PATH",env_path.c_str());
    if (e) {
        std::cerr << "`_putenv_s` failed, returned " << e << std::endl;
        exit(EXIT_FAILURE);
    }
    std::cout << std::endl;
//    e = std::system("echo \"In child process `PATH`=%PATH%\"");
    if (e) {
        std::cerr << "`std::system` failed, returned " << e << std::endl;
        exit(EXIT_FAILURE);
    }
#endif


    string CMD1 = "7z h -scrc ";
    string CMD2 = DirectoryPatch.toStdString();
    string CMD3 = " | grep \"CRC32  for data:\"";
    string CMD = CMD1 + CMD2 + CMD3;
//    char buf[BUFSIZ];
    char buf[40];
    FILE *ptr;

    if ((ptr = popen(CMD.c_str(), "r")) != NULL)
            while (fgets(buf, 40, ptr) != NULL){}
//                    (void) printf("%s", buf);
                (void) pclose(ptr);
    string tmp{buf};
    string result = tmp.substr(30, 8);
return   result;
}



#pragma once
#include "main.h"

ostream& operator<<(ostream &os, const chrono::time_point<chrono::system_clock> &t){
    const auto tt   (chrono::system_clock::to_time_t(t));
    const auto loct (std::localtime(&tt));
    return os << put_time(loct, "%Y-%m-%d");//%c
}
using days = chrono::duration<chrono::hours::rep, ratio_multiply<chrono::hours::period, ratio<24>>>;
constexpr days operator ""_days(unsigned long long d){
    return days{d};
}

bool parseJSON(string & patchToFile){
    QFile file(QString::fromStdString(patchToFile));
    file.open(QIODevice::ReadOnly);
    QString jsonData = file.readAll();
    file.close();

    //Переменные
    std::string naimenovanieIzdeliya{""};
    std::string naimenovanieDokumenta{""};
    std::string oboznachenieIkodDokumenta{""};
    int numberSheets{-1};
    std::string company{""};
    std::string creater{""};
    auto createdData = std::chrono::system_clock::now();
    std::string createdDataStr{""};
    std::string infoOrderList{""};
    int changeNum{-1};
    std::string changeNotificationNum{""};
    auto notificationData = std::chrono::system_clock::now();
    std::string notificationDataStr{""};
    std::string inventoryNumOriginal{""};
    auto storageData = std::chrono::system_clock::now();
    std::string storageDataStr{""};
    std::string litera{""};
    int dokumentCode{-1};
    int version{-1};
    std::string algorithm{""};
    std::string contromSummOrigin{""};
    std::string contromSummParts{""};
    std::string software{""};
    int volumeInSheets{-1};
    bool isValid{false};
    std::string tags{""};

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
                    cout << "\tНаименование изделия: " << naimenovanieIzdeliya << "\n";
                }
                if (requisites.HasMember("Наименование документа")){
                    naimenovanieDokumenta = requisites["Наименование документа"].GetString();
                    cout << "\tНаименование документа:" << naimenovanieDokumenta << "\n";
                }
                if (requisites.HasMember("Обозначение и код документа")){
                    oboznachenieIkodDokumenta = requisites["Обозначение и код документа"].GetString();
                    cout << "\tОбозначение и код документа:" << oboznachenieIkodDokumenta << "\n";
                }
                if (requisites.HasMember("Общее количество листов документа")){
                    numberSheets = requisites["Общее количество листов документа"].GetInt();
                    cout << "\tОбщее количество листов документа:" << numberSheets << "\n";
                }
                if (requisites.HasMember("Наименование или код организации")){
                    company = requisites["Наименование или код организации"].GetString();
                    cout << "\tНаименование или код организации:" << company << "\n";
                }
                if (requisites.HasMember("Сведения о подписании документа")){
                    cout << "\tСведения о подписании документа:\n";
                    const   Value & infoAboutSigning = requisites["Сведения о подписании документа"];
                    if (infoAboutSigning.HasMember("Разработал")){
                        creater = infoAboutSigning["Разработал"].GetString();
                        cout << "\t\tРазработал: " << creater << "\n";
                    }
                    if (infoAboutSigning.HasMember("Дата")){
                        createdDataStr = infoAboutSigning["Дата"].GetString();
                        std::stringstream ss(createdDataStr);
                        std::tm tm = {};
                        ss >> std::get_time(&tm, "%Y-%m-%d");
                        createdData = std::chrono::system_clock::from_time_t(std::mktime(&tm));
                        cout << "\t\tДата: " << createdData << "\n";
                    }
                    if (infoAboutSigning.HasMember("Информационно-удостоверяющий лист")){
                        infoOrderList = infoAboutSigning["Информационно-удостоверяющий лист"].GetString();
                        cout << "\t\tИнформационно-удостоверяющий лист: " << infoOrderList << "\n";
                    }
                }
                if (requisites.HasMember("Номер изменения")){
                    changeNum = requisites["Номер изменения"].GetInt();
                    cout << "\tНомер изменения: " << changeNum << "\n";
                }
                if (requisites.HasMember("Номер извещения об изменении")){
                    changeNotificationNum = requisites["Номер извещения об изменении"].GetString();
                    cout << "\tНомер извещения об изменении: " << changeNotificationNum << "\n";
                }
                if (requisites.HasMember("Дата извещения об изменении")){
                    notificationDataStr = requisites["Дата извещения об изменении"].GetString();
                    std::stringstream ss(notificationDataStr);
                    std::tm tm = {};
                    ss >> std::get_time(&tm, "%Y-%m-%d");
                    notificationData = std::chrono::system_clock::from_time_t(std::mktime(&tm));
                    cout << "\tДата извещения об изменении: " << notificationData << "\n";
                }
                if (requisites.HasMember("Инвентарный номер подлинника")){
                    inventoryNumOriginal = requisites["Инвентарный номер подлинника"].GetString();
                    cout << "\tИнвентарный номер подлинника: " << inventoryNumOriginal << "\n";
                }
                if (requisites.HasMember("Дата приемки на хранение")){
                    storageDataStr = requisites["Дата приемки на хранение"].GetString();
                    std::stringstream ss(storageDataStr);
                    std::tm tm = {};
                    ss >> std::get_time(&tm, "%Y-%m-%d");
                    storageData = std::chrono::system_clock::from_time_t(std::mktime(&tm));
                    cout << "\tДата приемки на хранение: " << storageData << "\n";
                }
                if (requisites.HasMember("Литера")){
                    litera = requisites["Литера"].GetString();
                    if (litera == "")
                        cout << "\tЛитера: " << "\"\"" << "\n";
                    else
                        cout << "\tЛитера: " << litera << "\n";
                }
                if (requisites.HasMember("Код документа в зависимости от характера использования")){
                    dokumentCode = requisites["Код документа в зависимости от характера использования"].GetInt();
                    cout << "\tКод документа в зависимости от характера использования: " << dokumentCode << "\n";
                }
            }//Реквизиты документа

            if (document.HasMember("Сервисные данные")){
                cout << "   Сервисные данные:\n";
                const   Value & serviceData = document["Сервисные данные"];
                if (serviceData.HasMember("Версия формата реквизитной части")){
                    version = serviceData["Версия формата реквизитной части"].GetInt();
                    cout << "\tВерсия формата реквизитной части: " << version << "\n";
                }
                if (serviceData.HasMember("Алгоритм расчета контрольной суммы")){
                    algorithm = serviceData["Алгоритм расчета контрольной суммы"].GetString();
                    cout << "\tАлгоритм расчета контрольной суммы: " << algorithm << "\n";
                }
                if (serviceData.HasMember("Значение контрольной суммы подлинника")){
                    contromSummOrigin = serviceData["Значение контрольной суммы подлинника"].GetString();
                    cout << "\tЗначение контрольной суммы подлинника: " << contromSummOrigin << "\n";
                }
                if (serviceData.HasMember("Значение контрольной суммы содержательных частей")){
                    contromSummParts = serviceData["Значение контрольной суммы содержательных частей"].GetString();
                    cout << "\tЗначение контрольной суммы содержательных частей: " << contromSummParts << "\n";
                }
                if (serviceData.HasMember("Программное обеспечение для редактирования исходных данных")){
                    software = serviceData["Программное обеспечение для редактирования исходных данных"].GetString();
                    cout << "\tПрограммное обеспечение для редактирования исходных данных: " << software << "\n";
                }
                if (serviceData.HasMember("Объем в листах А4")){
                    volumeInSheets = serviceData["Объем в листах А4"].GetInt();
                    cout << "\tОбъем в листах А4: " << volumeInSheets << "\n";
                }
                if (serviceData.HasMember("Документ действителен")){
                    isValid = serviceData["Документ действителен"].GetBool();
                    cout << "\tДокумент действителен: " << boolalpha << isValid << "\n";
                }
                if (serviceData.HasMember("Теги")){
                    tags = serviceData["Теги"].GetString();
                    if (tags == "")
                        cout << "\tТеги: " << "\"\"" << "\n";
                    else
                        cout << "\tТеги: " << tags << "\n\n";
                }
            } //Сервисные данные
        }//document.IsObject()
    }//Parse
    else
    {
        //ошибка парсинга
        return false;
    }
return true;
}

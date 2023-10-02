#include <QCoreApplication>
#include <QFile>
#include <QDebug>
#include "rapidjson/document.h"
#include "rapidjson/error/en.h"
#include <chrono>
#include <iomanip>
#include <iostream>
#include <sstream>

using namespace rapidjson;
using namespace std;

ostream& operator<<(ostream &os, const chrono::time_point<chrono::system_clock> &t){
    const auto tt   (chrono::system_clock::to_time_t(t));
    const auto loct (std::localtime(&tt));
    return os << put_time(loct, "%Y-%m-%d");//%c
}
using days = chrono::duration<chrono::hours::rep, ratio_multiply<chrono::hours::period, ratio<24>>>;
constexpr days operator ""_days(unsigned long long d){
    return days{d};
}
using namespace chrono_literals;

int main(int argc, char *argv[])
{
    QFile file("../index.json");
    file.open(QIODevice::ReadOnly);
    QString jsonData = file.readAll();
    file.close();

    std::string naimenovanieIzdeliya{""};
    std::string naimenovanieDokumenta{""};
    std::string oboznachenieIkodDokumenta{""};
    int numberSheets{0};
    std::string company{""};
    std::string creater{""};
    std::tm tm = {};
    auto createdData = std::chrono::system_clock::now();
    std::string createdDataStr{""};
    std::string infoOrderList{""};

    std::string litera{""};

    Document document;
    if (!document.Parse(jsonData.toStdString().c_str()).HasParseError())
    {
        if (document.IsObject())
        {
            if (document.HasMember("Реквизиты документа по ГОСТ 2.104"))
            {
                qDebug() << "Реквизиты документа по ГОСТ 2.104";
                const   Value & requisites = document["Реквизиты документа по ГОСТ 2.104"];
                if (requisites.HasMember("Наименование изделия")){
                    naimenovanieIzdeliya = requisites["Наименование изделия"].GetString();
                    qDebug() << "\tНаименование изделия: " << naimenovanieIzdeliya.c_str();
                }
                if (requisites.HasMember("Наименование документа")){
                    naimenovanieDokumenta = requisites["Наименование документа"].GetString();
                    qDebug() << "\tНаименование документа:" << naimenovanieDokumenta.c_str();
                }
                if (requisites.HasMember("Обозначение и код документа")){
                    oboznachenieIkodDokumenta = requisites["Обозначение и код документа"].GetString();
                    qDebug() << "\tОбозначение и код документа:" << oboznachenieIkodDokumenta.c_str();
                }
                if (requisites.HasMember("Общее количество листов документа")){
                    numberSheets = requisites["Общее количество листов документа"].GetInt();
                    qDebug() << "\tОбщее количество листов документа:" << numberSheets;
                }
                if (requisites.HasMember("Наименование или код организации")){
                    company = requisites["Наименование или код организации"].GetString();
                    qDebug() << "\tНаименование или код организации:" << company.c_str();
                }
                if (requisites.HasMember("Сведения о подписании документа")){
                    qDebug() << "\tСведения о подписании документа:";
                    const   Value & infoAboutSigning = requisites["Сведения о подписании документа"];
                    if (infoAboutSigning.HasMember("Разработал")){
                        creater = infoAboutSigning["Разработал"].GetString();
                        qDebug() << "\t\tРазработал:" << creater.c_str();
                    }
                    if (infoAboutSigning.HasMember("Дата")){
                        createdDataStr = infoAboutSigning["Дата"].GetString();
                        std::stringstream ss(createdDataStr);
                        ss >> std::get_time(&tm, "%Y-%m-%d");
                        createdData = std::chrono::system_clock::from_time_t(std::mktime(&tm));
                        cout << "Дата разработки:"<< createdData <<"\n";
                        qDebug() << "\t\tДата:" << createdDataStr.c_str();
                    }
                    if (infoAboutSigning.HasMember("Информационно-удостоверяющий лист")){
                        infoOrderList = infoAboutSigning["Информационно-удостоверяющий лист"].GetString();
                        qDebug() << "\t\tИнформационно-удостоверяющий лист:" << infoOrderList.c_str();
                    }
                }
                if (requisites.HasMember("Номер изменения")){
                    qDebug() << "\tНомер изменения:" << requisites["Номер изменения"].GetInt();
                }
                if (requisites.HasMember("Номер извещения об изменении")){
                    qDebug() << "\tНомер извещения об изменении:" << requisites["Номер извещения об изменении"].GetString();
                }
                if (requisites.HasMember("Дата извещения об изменении")){
                    qDebug() << "\tДата извещения об изменении:" << requisites["Дата извещения об изменении"].GetString();
                }
                if (requisites.HasMember("Инвентарный номер подлинника")){
                    qDebug() << "\tИнвентарный номер подлинника:" << requisites["Инвентарный номер подлинника"].GetString();
                }
                if (requisites.HasMember("Дата приемки на хранение")){
                    qDebug() << "\tДата приемки на хранение:" << requisites["Дата приемки на хранение"].GetString();
                }
                if (requisites.HasMember("Литера")){
                    litera = requisites["Литера"].GetString();
                    if (litera == "")
                        qDebug() << "\tЛитера:" << "\"\"";
                    else
                        qDebug() << "\tЛитера:" << litera.c_str();
                }
                if (requisites.HasMember("Код документа в зависимости от характера использования")){
                    qDebug() << "\tКод документа в зависимости от характера использования:" << requisites["Код документа в зависимости от характера использования"].GetInt();
                }
            }
            if (document.HasMember("Сервисные данные")){
                qDebug() << "Сервисные данные:";
                const   Value & serviceData = document["Сервисные данные"];
                if (serviceData.HasMember("Версия формата реквизитной части")){
                    qDebug() << "\tВерсия формата реквизитной части: " << serviceData["Версия формата реквизитной части"].GetInt();
                }
                if (serviceData.HasMember("Алгоритм расчета контрольной суммы")){
                    qDebug() << "\tАлгоритм расчета контрольной суммы: " << serviceData["Алгоритм расчета контрольной суммы"].GetString();
                }
                if (serviceData.HasMember("Значение контрольной суммы подлинника")){
                    qDebug() << "\tЗначение контрольной суммы подлинника: " << serviceData["Значение контрольной суммы подлинника"].GetString();
                }
                if (serviceData.HasMember("Значение контрольной суммы содержательных частей")){
                    qDebug() << "\tЗначение контрольной суммы содержательных частей: " << serviceData["Значение контрольной суммы содержательных частей"].GetString();
                }
                if (serviceData.HasMember("Программное обеспечение для редактирования исходных данных")){
                    qDebug() << "\tПрограммное обеспечение для редактирования исходных данных: " << serviceData["Программное обеспечение для редактирования исходных данных"].GetString();
                }
                if (serviceData.HasMember("Объем в листах А4")){
                    qDebug() << "\tОбъем в листах А4: " << serviceData["Объем в листах А4"].GetInt();
                }
                if (serviceData.HasMember("Документ действителен")){
                    qDebug() << "\tДокумент действителен: " << serviceData["Документ действителен"].GetBool();
                }
                if (serviceData.HasMember("Теги")){
                    qDebug() << "\tТеги: " << serviceData["Теги"].GetString();
                }
            }
        }
    }




//    if (!document.Parse(jsonData.toStdString().c_str()).HasParseError())
//        {
//            if (document.IsObject())
//            {
//                if (document.HasMember("array")) // Получаем данные из массива array
//                {
//                    if (document["array"].IsArray())
//                    {
//                        Value::Array ds =  document["array"].GetArray();
//                        for (Value::ConstValueIterator itr = ds.Begin(); itr != ds.End(); ++itr) {
//                            const Value& attribute = *itr;
//                            qDebug() << attribute.GetInt();
//                        }
//                    }
//                }
//                if (document.HasMember("boolean")) // Получаем данные из boolean
//                {
//                    if (document["boolean"].IsBool())
//                    {
//                        qDebug() <<  document["boolean"].GetBool();
//                    }
//                }
//                if (document.HasMember("Наименование_изделия")) // Получаем данные из Наименование изделия
//                {
//                    if (document["Наименование_изделия"].IsString())
//                    {
//                        qDebug() <<  document["Наименование_изделия"].GetString();
//                    }
//                }
//                if (document.HasMember("null")) // Получаем данные из null
//                {
//                    if (document["null"].IsNull())
//                    {
//                        qDebug() <<  document["null"].GetType();
//                    }
//                }
//                if (document.HasMember("number")) // Получаем данные из number
//                {
//                    if (document["number"].IsInt())
//                    {
//                        qDebug() <<  document["number"].GetInt();
//                    }
//                }
//                if (document.HasMember("object")) // Получаем данные из object
//                {
//                    const   Value & a = document["object"];
//                    if (a.HasMember("a")) // Получаем данные из object a
//                    {
//                        qDebug() << a["a"].GetString();
//                    }
//                    if (a.HasMember("c")) // Получаем данные из object c
//                    {
//                        qDebug() << a["c"].GetString();
//                    }
//                }
//                if (document.HasMember("string")) // Получаем данные из string
//                {
//                    if (document["string"].IsString())
//                    {
//                        qDebug() <<  document["string"].GetString();
//                    }
//                }
//                if (document.HasMember("Test1")) // Получаем данные из Test1
//                {
//                    Value & a = document["Test1"];  // Получаем данные из массива T
//                    if (a.HasMember("T"))
//                    {
//                        if (a["T"].IsArray())
//                        {
//                            Value::Array ds =  a["T"].GetArray();
//                            for (Value::ConstValueIterator itr = ds.Begin(); itr != ds.End(); ++itr) {
//                                const Value& attribute = *itr;
//                                qDebug() << attribute.GetString();
//                            }
//                        }
//                    }
//                    if (a.HasMember("int"))  // Получаем данные из int
//                    {
//                        if (a["int"].IsInt())
//                        {
//                            qDebug() <<  a["int"].GetInt();
//                        }
//                    }
//                }
//            }
//        }
//        else {
//            qDebug() <<  document.GetErrorOffset();
//            qDebug() <<  GetParseError_En(document.GetParseError());
//        }




//    QByteArray ba = jsonData.toUtf8();
//    QJsonDocument jsonDocument = QJsonDocument::fromJson(ba);
//    QJsonObject jsonObject = jsonDocument.object();
//    QString FirstName = jsonObject["FirstName"].toString();
//    qDebug() << FirstName;

}

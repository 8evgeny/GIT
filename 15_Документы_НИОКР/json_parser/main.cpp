#include <QCoreApplication>
#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QDebug>
#include "rapidjson/document.h"
#include "rapidjson/error/en.h"

using namespace rapidjson;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QFile file("../index.json");
//    QFile file("../example.json");
    file.open(QIODevice::ReadOnly);
    QString jsonData = file.readAll();
    file.close();

    Document document;
    if (!document.Parse(jsonData.toStdString().c_str()).HasParseError())
    {
        if (document.IsObject())
        {
            if (document.HasMember("Реквизиты документа по ГОСТ 2.104"))
            {
                qDebug() << "Реквизиты документа по ГОСТ 2.104";
                const   Value & a = document["Реквизиты документа по ГОСТ 2.104"];
                if (a.HasMember("Наименование изделия")){
                    qDebug() << "\tНаименование изделия: " << a["Наименование изделия"].GetString();
                }
                if (a.HasMember("Наименование документа")){
                    qDebug() << "\tНаименование документа:" << a["Наименование документа"].GetString();
                }
                if (a.HasMember("Обозначение и код документа")){
                    qDebug() << "\tОбозначение и код документа:" << a["Обозначение и код документа"].GetString();
                }
                if (a.HasMember("Общее количество листов документа")){
                    qDebug() << "\tОбщее количество листов документа:" << a["Общее количество листов документа"].GetInt();
                }
                if (a.HasMember("Наименование или код организации")){
                    qDebug() << "\tНаименование или код организации:" << a["Наименование или код организации"].GetString();
                }
                if (a.HasMember("Сведения о подписании документа")){
                    qDebug() << "\tСведения о подписании документа:";
                    const   Value & b = a["Сведения о подписании документа"];
                    if (b.HasMember("Разработал")){
                        qDebug() << "\t\tРазработал:" << b["Разработал"].GetString();
                    }
                    if (b.HasMember("Дата")){
                        qDebug() << "\t\tДата:" << b["Дата"].GetString();
                    }
                    if (b.HasMember("Информационно-удостоверяющий лист")){
                        qDebug() << "\t\tИнформационно-удостоверяющий лист:" << b["Информационно-удостоверяющий лист"].GetString();
                    }
                }
                if (a.HasMember("Номер изменения")){
                    qDebug() << "\tНомер изменения:" << a["Номер изменения"].GetInt();
                }
                if (a.HasMember("Номер извещения об изменении")){
                    qDebug() << "\tНомер извещения об изменении:" << a["Номер извещения об изменении"].GetString();
                }
                if (a.HasMember("Дата извещения об изменении")){
                    qDebug() << "\tДата извещения об изменении:" << a["Дата извещения об изменении"].GetString();
                }
                if (a.HasMember("Инвентарный номер подлинника")){
                    qDebug() << "\tИнвентарный номер подлинника:" << a["Инвентарный номер подлинника"].GetString();
                }
                if (a.HasMember("Дата приемки на хранение")){
                    qDebug() << "\tДата приемки на хранение:" << a["Дата приемки на хранение"].GetString();
                }
                if (a.HasMember("Литера")){
                    qDebug() << "\tЛитера:" << a["Литера"].GetString();
                }
                if (a.HasMember("Код документа в зависимости от характера использования")){
                    qDebug() << "\tКод документа в зависимости от характера использования:" << a["Код документа в зависимости от характера использования"].GetInt();
                }
            }
            if (document.HasMember("Сервисные данные")){
                qDebug() << "Сервисные данные:";
                const   Value & c = document["Сервисные данные"];
                if (c.HasMember("Версия формата реквизитной части")){
                    qDebug() << "\tВерсия формата реквизитной части: " << c["Версия формата реквизитной части"].GetInt();
                }
                if (c.HasMember("Алгоритм расчета контрольной суммы")){
                    qDebug() << "\tАлгоритм расчета контрольной суммы: " << c["Алгоритм расчета контрольной суммы"].GetString();
                }
                if (c.HasMember("Значение контрольной суммы подлинника")){
                    qDebug() << "\tЗначение контрольной суммы подлинника: " << c["Значение контрольной суммы подлинника"].GetString();
                }
                if (c.HasMember("Значение контрольной суммы содержательных частей")){
                    qDebug() << "\tЗначение контрольной суммы содержательных частей: " << c["Значение контрольной суммы содержательных частей"].GetString();
                }
                if (c.HasMember("Программное обеспечение для редактирования исходных данных")){
                    qDebug() << "\tПрограммное обеспечение для редактирования исходных данных: " << c["Программное обеспечение для редактирования исходных данных"].GetString();
                }
                if (c.HasMember("Объем в листах А4")){
                    qDebug() << "\tОбъем в листах А4: " << c["Объем в листах А4"].GetInt();
                }
                if (c.HasMember("Документ действителен")){
                    qDebug() << "\tДокумент действителен: " << c["Документ действителен"].GetBool();
                }
                if (c.HasMember("Теги")){
                    qDebug() << "\tТеги: " << c["Теги"].GetString();
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

    a.exit(0);
}

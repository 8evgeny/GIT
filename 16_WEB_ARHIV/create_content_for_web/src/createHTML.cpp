#include "main.h"

string createHTML(vector <string> & content)
{
//    content.push_back(oboznachenieIkodDokumenta); 0
//    content.push_back(naimenovanieIzdeliya);      1
//    content.push_back(naimenovanieDokumenta);     2
//    content.push_back(changeNumStr);              3
//    content.push_back(notificationDataStr);       4
//    content.push_back(controlSummOrigin);         5
//    content.push_back(infoOrderList);             6

//    if (isValid)                                  7
//        content.push_back("Действующий");
//    if (!isValid)
//        content.push_back("Недействующий");
//    content.push_back(inventoryNumOriginal);      8
//    content.push_back(storageDataStr);            9
//    content.push_back(creater);                   10
//    content.push_back(crc32Main);                 11
//    content.push_back(crc32Contents);             12
//    if (litera == "")                             13
//        content.push_back("нет");
//    if (litera != "")
//        content.push_back(litera);
//    content.push_back(software);                  14
//    if (tags == "")                               15
//        content.push_back("нет");
//    if (tags != "")
//        content.push_back(tags);


    QString izdelie = QString::fromStdString(content[1]);

string html =
    "<!doctype html>"
    "<html lang='ru'>"
    "<head>"
    "<meta charset='utf-8'>"
    "<title>Карточка изделия</title>"
    "</head>"
    "<body>"
    "<h1 align='center' style='color:blue'>" + izdelie.toUpper().toStdString() + "</h1>"
    "<h1 >" + content[2] + "</h1>"
    "<h2 ><a href=\""+content[0]+".PDF\" style = \"text-decoration: none;\">"+content[0]+"</a></h2>"
    "<h2 > Статус документа: <font style='color:red'>" + content[7] + "</font ></h2>"
    "<h2 > Инвентарный номер: <font style='color:red'>" + content[8] + "</font ></h2>"
    "<h2 > Дата приемки на хранение: <font style='color:red'>" + content[9] + "</font ></h2>"
    "<h2 > Разработчик: <font style='color:red'>" + content[10] + "</font ></h2>"
    "<h2 > Информационно-удостоверяющий лист: <font style='color:red'>" + content[6] + ".pdf</font ></h2>"
    "<h2 > Контрольная сумма подлинника: <font style='color:red'>" + content[11] + "</font ></h2>"
    "<h2 > Контрольная сумма содержательных частей: <font style='color:red'>" + content[12] + "</font ></h2>"
    "<h2 > Литера: <font style='color:red'>" + content[13] + "</font ></h2>"
    "<h2 > Программное обеспечение для редактирования исходных данных: <font style='color:red'>" + content[14] + "</font ></h2>"
    "<h2 > Теги: <font style='color:red'>" + content[15] + "</font ></h2>"
    "</body>"
    "</html>"
;
        return html;
}

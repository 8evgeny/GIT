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
//    content.push_back(inventoryNumOriginal);      7

//    content.push_back(inventoryNumOriginal);      8
//    content.push_back(storageDataStr);            9
//    content.push_back(creater);                   10
//    content.push_back(crc32Main);                 11
//    content.push_back(crc32Contents);             12
//    if (litera == "")                             13
//        content.push_back("нет");
//    if (litera != "")
//        content.push_back(litera);



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
        "<h2 >" + content[2] + "</h2>"
        "<h2 ><a href=\""+content[0]+".PDF\" style = \"text-decoration: none;\">"+content[0]+"</a></h2>"


        "</body>"
        "</html>"
;
        return html;
}

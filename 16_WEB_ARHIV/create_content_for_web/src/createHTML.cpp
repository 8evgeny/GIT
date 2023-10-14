#include "main.h"

string createHTML(vector <string> & content)
{
//    content.push_back(oboznachenieIkodDokumenta);
//    content.push_back(naimenovanieIzdeliya);
//    content.push_back(naimenovanieDokumenta);
//    content.push_back(changeNumStr);
//    content.push_back(notificationDataStr);
//    content.push_back(controlSummOrigin);
//    content.push_back(infoOrderList);
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

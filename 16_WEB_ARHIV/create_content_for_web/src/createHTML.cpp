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
string html =
        "<!doctype html>"
        "<html lang='ru'>"
        "<head>"
        "<meta charset='utf-8'>"
        "<title>Изделие " + content[1] + "</title>"
        "</head>"
        "<body>"
        "<h1 align='center' style='color:blue'>НАИМЕНОВАНИЕ ИЗДЕЛИЯ</h1>"


        "</body>"
        "</html>"
;
        return html;
}

#include "main.h"

string createHTML(vector <string> & content, vector<pair<string,string>> & oldZipData)
{
    string tmp1 = "\"" + content[0] + ".PDF\"";
//    content.push_back(oboznachenieIkodDokumenta); 0
//    content.push_back(naimenovanieIzdeliya);      1
//    content.push_back(naimenovanieDokumenta);     2
//    content.push_back(changeNumStr);              3
//    content.push_back(notificationDataStr);       4
//    content.push_back(controlSummOrigin);         5
//    content.push_back(infoOrderList);             6
//    content.push_back("Действующий");             7
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
//    content.push_back(changeNotificationNum);     16
//    content.push_back(notificationDataStr);       17


string html =
    "<!doctype html>"
    "<html lang='ru'>"
    "<head>"
    "<meta charset='utf-8'>"
    "<title>Карточка изделия</title>"
    "</head>"
    "<style>"
    "table, th, td, thead{"
      "border:1px solid black;"
    "}"
    "a, u {"
      "text-decoration: none;"
    "}"
    "body {"
      "padding: 30px;"
    "}"
    "td, th {"
      "padding-left: 20px; padding-right: 20px;"
    "}"
    "thead {"
      "margin-bottom: 0px;"
    "}"
    "</style>"
    "<body>"
    "<h2 align='center' style='color:black'>" + QString::fromStdString(content[1]).toUpper().toStdString() + "</h2>"
    "<h3 align='center'>" + content[2] + "</h3>"
    "<h3 align='center'><a href=" + tmp1 + ">" + content[0] + "</a></h3>"
    "<table width='40%'>"                                          //cellpadding='4' cellspacing='0'
        "<tr><thead ><h2 align='left'>Учетные данные</h2></thead>"
        "<tr>"
            "<td><h3 align='left'>Статус документа</h3></td>"
            "<td><h3 align='center'>" + content[7] + "</h3></td>"
        "</tr>"
        "<tr>"
          "<td><h3 align='left'>Инвентарный номер</h3></td>"
          "<td><h3 align='center'>" + content[8] + "</h3></td>"
        "</tr>"
        "<tr>"
          "<td><h3 align='left'>Дата приемки на хранение</h3></td>"
          "<td><h3 align='center'>" + content[9] + "</h3></td>"
        "</tr>"
    "</table>"
    "<br>"
    "<table style='width: 40%;'>"
       "<tr><thead><h2 align='left'>Сведения о документе</h2></thead>"
       "<tr>"
           "<td><h3 align='left'>Разработчик</h3></td>"
           "<td><h3 align='center'>" + content[10] + "</h3></td>"
       "</tr>"
       "<tr>"
         "<td><h3 align='left'>Информационно-удостоверяющий лист</h3></td>"
         "<td><h3 align='center'><a href='" + content[6] + ".PDF'>" + content[6] + "</a></h3></td>"
       "</tr>"
       "<tr>"
         "<td><h3 align='left'>Контрольная сумма подлинника</h3></td>"
         "<td><h3 align='center'>" + content[11] + "</h3></td>"
       "</tr>"
       "<tr>"
         "<td><h3 align='left'>Контрольная сумма содержательных частей</h3></td>"
         "<td><h3 align='center'>" + content[12] + "</h3></td>"
       "</tr>"
       "<tr>"
         "<td><h3 align='left'>Литера</h3></td>"
         "<td><h3 align='center'>" + content[13] + "</h3></td>"
       "</tr>"
       "<tr>"
         "<td><h3 align='left'>Программное обеспечение для редактирования исходных данных</h3></td>"
         "<td><h3 align='center'>" + content[14] + "</h3></td>"
       "</tr>"
       "<tr>"
         "<td><h3 align='left'>Теги</h3></td>"
         "<td><h3 align='center'>" + content[15] + "</h3></td>"
       "</tr>"
    "</table>"
    "<br>"
    "<table style='width: 70%;'>"
        "<tr><thead><h2 align='left'>Сведения об изменениях документа</h2></thead>"
            "<th><h3 align='center'>Номер изменения</h3></th>"
            "<th><h3 align='center'>Извещение об изменении</h3></th>"
            "<th width='20%'><h3 align='center'>Дата</h3></th>"
            "<th width='30%'><h3 align='center'>Ссылка на документ соответствующего номера изменения</h3></th>"
        "</tr>"
        ;
    uint numIzm = atoi(content[3].c_str()); //Номер изма
    string html2 = "";
    string tmp = "";
    html2 =
            "<tr>"
                "<td><h3 align='center'>" + content[3] + "</h3></td>"
                "<td><h3 align='center'><a href='" + content[16] + ".PDF'>" + content[16] + "</a></h3></td>"
                "<td><h3 align='center'>" + content[17] + "</h3></td>"
                "<td><h3 align='center'><a href='" + content[0] + ".изм" + content[3] + ".zip'>" + content[0] + ".изм" + content[3] +  ".zip</a></a></h3></td>"
            "</tr>"
    ;
    for (auto i = oldZipData.size(); i > 0; --i)
    {
        tmp =
                "<tr>"
                    "<td><h3 align='center'>" + to_string(i - 1) + "</h3></td>"
                    "<td><h3 align='center'><a href='" + oldZipData[i - 1].first + ".PDF'>" + oldZipData[i - 1].first + "</a></h3></td>"
                    "<td><h3 align='center'>" + oldZipData[i - 1].second + "</h3></td>"
                    "<td><h3 align='center'><a href='" + content[0] + ".изм" + to_string(i-1) + ".zip'>" + content[0] + ".изм" + to_string(i-1) + ".zip</a></h3></td>"
                "</tr>"
                ;
        html2.append(tmp);
    }

    string html3 = "</table></body></html>";
    html.append(html2);
    html.append(html3);
return html;
}


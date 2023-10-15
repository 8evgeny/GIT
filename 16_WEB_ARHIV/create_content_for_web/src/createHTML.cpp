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
//    content.push_back(changeNotificationNum);     16
//    content.push_back(notificationDataStr);       17

    QString izdelie = QString::fromStdString(content[1]);

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
    "</style>"
    "<body>"
    "<h1 align='center' style='color:black'>" + izdelie.toUpper().toStdString() + "</h1>"
    "<h2 align='center'>" + content[2] + "</h2>"
    "<h2 align='center'><a href=" + content[0] + ".PDF>" + content[0] + "</a></h2>"

    "<table style='width: 40%;'>"
        "<tr><thead><h2 align='left'>Учетные данные</h2></thead>"
        "<tr>"
            "<td><h2 align='left'>Статус документа</h2></td>"
            "<td><h2 align='center'>" + content[7] + "</h2></td>"
        "</tr>"
        "<tr>"
          "<td><h2 align='left'>Инвентарный номер</h2></td>"
          "<td><h2 align='center'>" + content[8] + "</h2></td>"
        "</tr>"
        "<tr>"
          "<td><h2 align='left'>Дата приемки на хранение</h2></td>"
          "<td><h2 align='center'>" + content[9] + "</h2></td>"
        "</tr>"
    "</table>"
    "<br>"

    "<table style='width: 40%;'>"
       "<tr><thead><h2 align='left'>Сведения о документе</h2></thead>"
       "<tr>"
           "<td><h2 align='left'>Разработчик</h2></td>"
           "<td><h2 align='center'>" + content[10] + "</h2></td>"
       "</tr>"
       "<tr>"
         "<td><h2 align='left'>Информационно-удостоверяющий лист</h2></td>"
         "<td><h2 align='center'>"+ content[6] + "</h2></td>"
       "</tr>"
       "<tr>"
         "<td><h2 align='left'>Контрольная сумма подлинника</h2></td>"
         "<td><h2 align='center'>" + content[11] + "</h2></td>"
       "</tr>"
       "<tr>"
         "<td><h2 align='left'>Контрольная сумма содержательных частей</h2></td>"
         "<td><h2 align='center'>" + content[12] + "</h2></td>"
       "</tr>"
       "<tr>"
         "<td><h2 align='left'>Литера</h2></td>"
         "<td><h2 align='center'>" + content[13] + "</h2></td>"
       "</tr>"
       "<tr>"
         "<td><h2 align='left'>Программное обеспечение для редактирования исходных данных</h2></td>"
         "<td><h2 align='center'>" + content[14] + "</h2></td>"
       "</tr>"
       "<tr>"
         "<td><h2 align='left'>Теги</h2></td>"
         "<td><h2 align='center'>" + content[15] + "</h2></td>"
       "</tr>"
    "</table>"
    "<br>"


    "<table style='width: 60%;'>"
        "<tr><thead><h2 align='left'>Сведения об изменениях документа</h2></thead>"
            "<th><h2 align='center'>Номер изменения</h2></th>"
            "<th><h2 align='center'>Извещение об изменении</h2></th>"
            "<th width='20%'><h2 align='center'>Дата</h2></th>"
            "<th width='30%'><h2 align='center'>Ссылка на документ соответствующего номера изменения</h2></th>"
        "</tr>"
        "<tr>"
          "<td><h2 align='center'>" + content[3] + "</h2></td>"
          "<td><h2 align='center'>" + content[16] + "</h2></td>"
          "<td><h2 align='center'>" + content[17] + "</h2></td>"
          "<td><h2 align='center'><a href='" + content[16] + ".PDF'>" + content[16] + "</a></h2></td>"
        "</tr>"
    "</table>"



    "</body>"
    "</html>"
;
        return html;
}

const download = (path, filename) => {
    // Create a new link
    const anchor = document.createElement('a');
    anchor.href = path;
    anchor.download = filename;

    // Append to the DOM
    document.body.appendChild(anchor);

    // Trigger `click` event
    anchor.click();

    // Remove element from DOM
    document.body.removeChild(anchor);
};

function sleep(milliseconds) {
    const date = Date.now();
    let currentDate = null;
    do {
        currentDate = Date.now();
    } while (currentDate - date < milliseconds);
};

var state = 0;
var numberDoc;
var rule_arr;
var rule_arr1;
var rule_arr2;
var rule_arr3;
var rule_arr4;
var rule_arr5;
var rowData;
var creator;
var numRow;
var footer;
//var reqTableSize = new XMLHttpRequest();

//Правила сортировки - соответствуют файлам с порядком папок и данными по строкам
// rule_oboznachenieikoddokumenta ASC                               ruleSort_1
// rule_naimenovanieizdeliya ASC, notificationdatastr DESC          ruleSort_2
// rule_naimenovaniedokumenta ASC, notificationdatastr DESC         ruleSort_3
// rule_changenumstr DESC, oboznachenieikoddokumenta ASC            ruleSort_4
// rule_notificationdatastr DESC, oboznachenieikoddokumenta ASC     ruleSort_5

let reqSortRule1 = new XMLHttpRequest();
reqSortRule1.open('get', 'content_for_web/ruleSort_1', false);
let reqSortRule2 = new XMLHttpRequest();
reqSortRule2.open('get', 'content_for_web/ruleSort_2', false);
let reqSortRule3 = new XMLHttpRequest();
reqSortRule3.open('get', 'content_for_web/ruleSort_3', false);
let reqSortRule4 = new XMLHttpRequest();
reqSortRule4.open('get', 'content_for_web/ruleSort_4', false);
let reqSortRule5 = new XMLHttpRequest();
reqSortRule5.open('get', 'content_for_web/ruleSort_5', false);
reqSortRule1.send(null);
reqSortRule2.send(null);
reqSortRule3.send(null);
reqSortRule4.send(null);
reqSortRule5.send(null);
rule_arr1 = reqSortRule1.responseText.split('\n');
rule_arr2 = reqSortRule2.responseText.split('\n');
rule_arr3 = reqSortRule3.responseText.split('\n');
rule_arr4 = reqSortRule4.responseText.split('\n');
rule_arr5 = reqSortRule5.responseText.split('\n');

rule_arr = rule_arr1;

newTable();
//handleSortChange(); // Это лишнее - таблица лишний раз формируется
handleCreator();
createFooter();

function handleCreator() {
    selectElement = document.getElementById('creator');
    selectedCreator = selectElement.value;
    switch (selectedCreator) {
        case "creatorAll":
            creator = "all";
            break;
        case "creator1":
            creator = "creator1";
            break;
        case "creator2":
            creator = "creator2";
            break;
        case "creator3":
            creator = "creator3";
            break;
        case "creator4":
            creator = "creator4";
            break;
        case "creator5":
            creator = "creator5";
            break;
        case "creator6":
            creator = "creator6";
            break;
        case "creator7":
            creator = "creator7";
            break;
        case "creator8":
            creator = "creator8";
            break;
        case "creator9":
            creator = "creator9";
            break;
        case "creator10":
            creator = "creator10";
            break;
    }
    delTable();
    newTable();
    createTable();
}

function handleSortChange() {
    selectElement = document.getElementById('sort');
    selectedSort = selectElement.value;
    switch (selectedSort) {
        case "sort1":
            rule_arr = rule_arr1;
            state = 1;
            console.log("state: %d", state);
            break;
        case "sort2":
            rule_arr = rule_arr2;
            state = 2;
            console.log("state: %d", state);
            break;
        case "sort3":
            rule_arr = rule_arr3;
            state = 3;
            console.log("state: %d", state);
            break;
        case "sort4":
            rule_arr = rule_arr4;
            state = 4;
            console.log("state: %d", state);
            break;
        case "sort5":
            rule_arr = rule_arr5;
            state = 5;
            console.log("state: %d", state);
            break;
    }
    //в каждом элементе массива список папок согласно правилу сортировки
    // for (let i = 0; i<rule_arr.length; i++) {
    //     console.log ("%s\n", rule_arr[i]);
    // }
    delTable();
    newTable();
    createTable();
}

var tbl;
function newTable(){
    tbl = document.createElement("table");
    tbl.style.margin = 'auto';
    tbl.style.width = '1800px';
    tbl.style.border = '1px solid black';
}

function createFooter() {
    console.log ("createFooter");
     footer = document.createElement("p");        // создаем заголовок <h1>
    const  headerText = document.createTextNode("Created in GIT NIOKR, programmer Evgeny Parubets"); // создаем текстовый узел
    footer.appendChild( headerText); // добавляем в элемент h1 текстовый узел
    document.body.appendChild(footer);

}

//function deleteFooter() {
//    console.log ("deleteFooter");

//}

function delTable(){
//    deleteFooter();
    tbl.remove();

}

function createTable() {
    numberDoc = rule_arr.length;
    let numFolder
    numRow = 0;

//    for (let i = 0; i < numberDoc; i++) {
//        console.log("%s\n", rule_arr[i]);
//    }

    for (let i = 0; i <= numberDoc - 1; i++) {
        //Здесь получаем файл 'content_for_web/i/rowContent  для заполнения ряда
        let row = new XMLHttpRequest();
        if (i !== 0) {
            numFolder = rule_arr[i - 1];
            row.open('get', 'content_for_web/' + `${numFolder}` + '/' + "rowContent", false);
            row.onloadend = function() {
                if (row.readyState === 4) {
                    rowData = row.responseText.toString().split('\n');
//console.log("%s\n", rowData[7]);
                }
            }
            row.send(null);
        }

if (i === 0) { // Шапка таблицы
        tr = tbl.insertRow();
        let j;
        for (j = 0; j < 8; j++) {

                const td = tr.insertCell();
                tr.style.height = '80px';
                td.style.border = '1px solid black';
                td.style.fontWeight = 'bold';
                td.style.fontFamily = 'Arial';
                td.style.fontSize = '20px';
                td.style.textAlign = "center";
                td.style.fontStyle = "italic";
                td.style.padding = '10px';
                switch (j) {
                    case 0:
                        td.style.width = '40px';
                        td.appendChild(document.createTextNode(" Номер "));
                        break;
                    case 1:
                        td.style.width = '200px';
                        td.appendChild(document.createTextNode(`Обозначение и код документа`));
                        break;
                    case 2:
                        td.appendChild(document.createTextNode(`Наименование изделия`));
                        break;
                    case 3:
                        td.style.width = '240px';
                        td.appendChild(document.createTextNode(`Наименование документа`));
                        break;
                    case 4:
                        td.style.width = '60px';
                        td.appendChild(document.createTextNode(`Номер изменения`));
                        break;
                    case 5:
                        td.style.width = '220px';
                        td.appendChild(document.createTextNode(`Дата извещения об изменении`));
                        break;
                    case 6:
                        td.style.width = '220px';
                        td.appendChild(document.createTextNode(`Значение контрольной суммы подлинника`));
                        break;
                    case 7:
                        td.style.width = '270px';
                        td.appendChild(document.createTextNode(`Информационно - удостоверяющий лист`));
                        break;
            }
       }
  }


if (i > 0) {
if ((creator === "all")||
    (creator === "creator1" && rowData[7].startsWith("Василенков")) ||
    (creator === "creator2" && rowData[7].startsWith("Молчанов"))   ||
    (creator === "creator3" && rowData[7].startsWith("Потапов"))    ||
    (creator === "creator4" && rowData[7].startsWith("Давыдов"))    ||
    (creator === "creator5" && rowData[7].startsWith("Пяткин"))     ||
    (creator === "creator6" && rowData[7].startsWith("Соколов"))    ||
    (creator === "creator7" && rowData[7].startsWith("Солдатов"))   ||
    (creator === "creator8" && rowData[7].startsWith("Суриков"))    ||
    (creator === "creator9" && rowData[7].startsWith("Белоконев"))  ||
    (creator === "creator10" && rowData[7].startsWith("Парубец"))
    ) {
        tr = tbl.insertRow();
        ++numRow;
        for (let j = 0; j < 8; j++) {

                const td = tr.insertCell();
                td.style.border = '1px solid black';
                td.style.padding = '10px';
                td.style.fontSize = '18px';
                switch (j) {
                    case 0:
                        td.appendChild(document.createTextNode(numRow));
                        td.style.fontWeight = 'bold';
                        td.style.textAlign = "center";
                        td.style.width = '40px';
                        break;
                    case 1:
                        td.style.width = '200px';
                        td.style.textAlign = "left";
                        let oboznIkodDoc = document.createElement('a');
                        let linkObozn = document.createTextNode(rowData[0]);
                        oboznIkodDoc.style.textDecoration = "none";
                        oboznIkodDoc.appendChild(linkObozn);
                        oboznIkodDoc.title = rowData[0];
                        oboznIkodDoc.href = 'content_for_web/' + `${numFolder}` + '/' + `${rowData[0]}` + '.PDF';
                        td.appendChild(oboznIkodDoc);
                        //Загрузка контента как файла
                        //download('content_for_web/'+`${i}`+'/' + `${rowData[0]}` + '.PDF',`${rowData[0]}`+ '.pdf');
                        break;
                    case 2:
                        td.style.textAlign = "left";
                        let naimenovanieIzd = document.createElement('a');
                        let linkNaimen = document.createTextNode(rowData[1]);
                        naimenovanieIzd.style.textDecoration = "none";
                        naimenovanieIzd.appendChild(linkNaimen);
                        naimenovanieIzd.title = rowData[1];
                        naimenovanieIzd.href = 'content_for_web/' + `${numFolder}` + '/' + "document.html";
                        td.appendChild(naimenovanieIzd);
                        break;
                    case 3:
                        td.style.width = '240px';
                        td.style.textAlign = "left";
                        td.appendChild(document.createTextNode(`${rowData[2]}`));
                        break;
                    case 4:
                        td.style.width = '60px';
                        td.style.textAlign = "center";
                        td.appendChild(document.createTextNode(`${rowData[3]}`));
                        break;
                    case 5:
                        td.style.width = '220px';
                        td.style.textAlign = "center";
                        td.appendChild(document.createTextNode(`${rowData[4]}`));
                        break;
                    case 6:
                        td.style.width = '220px';
                        td.style.textAlign = "center";
                        td.appendChild(document.createTextNode(`${rowData[5]}`));
                        break;
                    case 7:
                        td.style.width = '270px';
                        td.style.textAlign = "left";
                        let orderList = document.createElement('a');
                        let linkOrder = document.createTextNode(rowData[6]);
                        orderList.style.textDecoration = "none";
                        orderList.appendChild(linkOrder);
                        orderList.title = rowData[6];
                        orderList.href = 'content_for_web/' + `${numFolder}` + '/' + `${rowData[6]}` + '.PDF';
                        td.appendChild(orderList);
                        break;
                }
         }
    }
}
    } //Строки
    document.body.appendChild(tbl);
    document.body.insertBefore(tbl, footer);



} //function createTable()




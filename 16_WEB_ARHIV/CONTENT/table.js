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
var rowData;
let reqTableSize = new XMLHttpRequest();

reqTableSize.open('get', 'content_for_web/numDoc', false);
reqTableSize.onreadystatechange = createTable;
reqTableSize.send(null);

function createTable() {

    if (reqTableSize.readyState === 4) {
        let numberDoc = reqTableSize.responseText.toString();
        const body = document.body,
        tbl = document.getElementById('table');
        tbl.style.margin = 'auto';
        tbl.style.width = '1800px';
        tbl.style.border = '1px solid black';

        for (let i = 0; i <= numberDoc; i++) {
            //Здесь получаем файл 'content_for_web/i/rowContent  для заполнения ряда
            let row = new XMLHttpRequest();

            if (i !== 0) {
                row.open('get', 'content_for_web/' + `${i}` + '/' + "rowContent", false);
                row.onloadend = function () {
                    if (row.readyState === 4) {
                        rowData = row.responseText.toString().split('\n');
                    }
                }
                row.send(null);
            }

            function fillRow() {
                if (row.readyState === 4) {
                    rowData = row.responseText.toString().split('\n');
                }
            }

            const tr = tbl.insertRow();

            if (i !== 0) tr.style.height = '40px';
            else tr.style.height = '80px';
            for (let j = 0; j < 8; j++) {
                if (i === 0) {
                    const td = tr.insertCell();
                    td.style.border = '1px solid black';
                    td.style.fontWeight = 'bold';
                    td.style.fontFamily = 'Arial';
                    td.style.fontSize = '18px';
                    td.style.textAlign = "center";
                    td.style.fontStyle = "italic";
                    switch (j) {
                        case 1:
                            td.appendChild(document.createTextNode(`Обозначение и код документа`));
                            break;
                        case 2:
                            td.appendChild(document.createTextNode(`Наименование изделия`));
                            break;
                        case 3:
                            td.appendChild(document.createTextNode(`Наименование документа`));
                            break;
                        case 4:
                            td.appendChild(document.createTextNode(`Номер изменения`));
                            break;
                        case 5:
                            td.appendChild(document.createTextNode(`Дата извещения об изменении`));
                            break;
                        case 6:
                            td.appendChild(document.createTextNode(`Значение контрольной суммы подлинника`));
                            break;
                        case 7:
                            td.appendChild(document.createTextNode(`Информационно - удостоверяющий лист`));
                            break;
                        case 0:
                            td.appendChild(document.createTextNode(" Номер "));
                            break;
                    }
                } // Шапка таблицы  if (i == 0)


                if (i !== 0) { //Номера по порядку
                    if (j === 0) {
                        const td = tr.insertCell();
                        td.appendChild(document.createTextNode(i));
                        td.style.fontWeight = 'bold';
                        td.style.border = '1px solid black';
                        td.style.textAlign = "center";
                        td.style.width = '80px';
                    } else {
                        // if (j == 7)
                        // {td.style.width = '200px';}

                        const td = tr.insertCell();
                        td.style.border = '1px solid black';
                        switch (j) {
                            case 1:
                                td.style.padding = '10px';
                                td.style.width = '200px';
                                td.style.fontSize = '18px';
                                let oboznIkodDoc = document.createElement('a');
                                let linkObozn = document.createTextNode(rowData[0]);
                                oboznIkodDoc.style.textDecoration = "none";
                                oboznIkodDoc.appendChild(linkObozn);
                                oboznIkodDoc.title = rowData[0];
                                oboznIkodDoc.href = 'content_for_web/' + `${i}` + '/' + `${rowData[0]}` + '.PDF';
                                td.appendChild(oboznIkodDoc);
                                //Загрузка контента как файла                              
                                //download('content_for_web/'+`${i}`+'/' + `${rowData[0]}` + '.PDF',`${rowData[0]}`+ '.pdf');
                                break;
                            case 2:
                                td.style.padding = '10px';
                                td.style.fontSize = '20px';
                                let naimenovanieIzd = document.createElement('a');
                                let linkNaimen = document.createTextNode(rowData[1]);
                                naimenovanieIzd.style.textDecoration = "none";
                                naimenovanieIzd.appendChild(linkNaimen);
                                naimenovanieIzd.title = rowData[1];
                                naimenovanieIzd.href = 'content_for_web/' + `${i}` + '/' + "document.html";
                                td.appendChild(naimenovanieIzd);
                                break;
                            case 3:
                                td.style.padding = '10px';
                                td.style.width = '240px';
                                td.style.fontSize = '18px';
                                td.appendChild(document.createTextNode(`${rowData[2]}`));
                                break;
                            case 4:
                                td.style.textAlign = "center";
                                td.style.width = '120px';
                                td.style.fontSize = '18px';
                                td.appendChild(document.createTextNode(`${rowData[3]}`));
                                break;
                            case 5:
                                td.style.textAlign = "center";
                                td.style.width = '200px';
                                td.style.fontSize = '18px';
                                td.appendChild(document.createTextNode(`${rowData[4]}`));
                                break;
                            case 6:
                                td.style.textAlign = "center";
                                td.style.width = '190px';
                                td.style.fontSize = '18px';
                                td.appendChild(document.createTextNode(`${rowData[5]}`));
                                break;
                            case 7:
                                td.style.textAlign = "center";
                                td.style.width = '270px';
                                td.style.fontSize = '18px';
                                let orderList = document.createElement('a');
                                let linkOrder = document.createTextNode(rowData[6]);
                                orderList.style.textDecoration = "none";
                                orderList.appendChild(linkOrder);
                                orderList.title = rowData[6];
                                orderList.href = 'content_for_web/' + `${i}` + '/' + `${rowData[6]}` + '.PDF';
                                td.appendChild(orderList);
                                break;

                        }


                    }


                }
            } //Заполнение столбцов  for (let j = 0; j < 8; j++)

        } //Строки
        body.appendChild(tbl);
    }

} //function createTable()




// console.log ("Done");

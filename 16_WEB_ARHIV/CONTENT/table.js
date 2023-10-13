function sleep(milliseconds) {
    const date = Date.now();
    let currentDate = null;
    do {
        currentDate = Date.now();
    } while (currentDate - date < milliseconds);
};
const dataTable = [];
let reqTableSize = new XMLHttpRequest();

reqTableSize.open('get', 'content_for_web/numDoc', false);
reqTableSize.onreadystatechange = createTable;
reqTableSize.send(null);

function createTable() {
    if (reqTableSize.readyState == 4) {
        let numberDoc = reqTableSize.responseText.toString();
        const body = document.body,
            tbl = document.getElementById('table');
        tbl.style.margin = 'auto';
        tbl.style.width = '1800px';
        tbl.style.border = '1px solid black';



        for (let i = 0; i <= numberDoc; i++) {
            //Здесь получаем файл 'content_for_web/i/rowContent  для заполнения ряда
            let row = new XMLHttpRequest();

            if (i != 0) {
                row.open('get', 'content_for_web/' + `${i}` + '/' + "rowContent", false);
                row.onloadend = fillRow;
                row.send(null);
            }

            function fillRow() {
                if (row.readyState == 4) {
                    let text = row.responseText.toString().split('\n');
                    dataTable[0] = text[0];
                    dataTable[1] = text[1];
                    dataTable[2] = text[2];
                    dataTable[3] = text[3];
                    dataTable[4] = text[4];
                    dataTable[5] = text[5];
                    dataTable[6] = text[6];

                }
            }

            const tr = tbl.insertRow();
            if (i!=0) tr.style.height='40px';
            else tr.style.height='80px';
            for (let j = 0; j < 8; j++) {
                if (i == 0) {
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


                if (i != 0) { //Номера по порядку
                    if (j == 0) {
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
                        if (j==4 || j==5 || j==6 || j ==7)td.style.textAlign = "center";
                        // console.log(j);
                        switch (j) {
                            case 1:
                                td.style.width = '200px';
                                td.appendChild(document.createTextNode(`${dataTable[0]}`));
                                break;
                            case 2:
                                td.appendChild(document.createTextNode(`${dataTable[1]}`));
                                break;
                            case 3:
                                td.style.width = '230px';
                                td.appendChild(document.createTextNode(`${dataTable[2]}`));
                                break;
                            case 4:
                                td.style.width = '120px';
                                td.appendChild(document.createTextNode(`${dataTable[3]}`));
                                break;
                            case 5:
                                td.style.width = '200px';
                                td.appendChild(document.createTextNode(`${dataTable[4]}`));
                                break;
                            case 6:
                                td.style.width = '190px';
                                td.appendChild(document.createTextNode(`${dataTable[5]}`));
                                break;
                            case 7:
                                td.style.width = '250px';
                                td.appendChild(document.createTextNode(`${dataTable[6]}`));
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
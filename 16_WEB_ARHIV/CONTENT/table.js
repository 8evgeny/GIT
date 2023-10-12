function sleep(milliseconds) {
    const date = Date.now();
    let currentDate = null;
    do {
        currentDate = Date.now();
    } while (currentDate - date < milliseconds);
};

let reqTableSize = new XMLHttpRequest();

reqTableSize.open('get', 'content_for_web/numDoc');
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
            //Здесь получаем файл 'content_for_web/i/i  для заполнения ряда
            let row = new XMLHttpRequest();

            if (i != 0) {
                row.open('get', 'content_for_web/' + `${i}` + '/' + `${i}`);
                row.onreadystatechange = fillRow;
                row.send(null);
            }

            function fillRow() {
                if (row.readyState == 4) {
                    let text = row.responseText.toString().split('\n');
                    console.log(text[1]);
                }
            }

            const tr = tbl.insertRow();
            for (let j = 0; j < 8; j++) {
                if (i == 0) {
                    const td = tr.insertCell();
                    td.style.border = '1px solid black';
                    td.style.fontWeight = 'bold';
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
                            td.appendChild(document.createTextNode(`Информационно-удостоверяющий лист`));
                            break;
                        case 0:
                            td.appendChild(document.createTextNode(`Порядковый номер`));
                            break;
                    }
                } // Шапка таблицы  if (i == 0)


                if (i != 0) {
                    if (j == 0) {
                        const td = tr.insertCell();
                        td.appendChild(document.createTextNode(i));
                        td.style.border = '1px solid black';
                        td.style.textAlign = "center";
                    } else {


                        const td = tr.insertCell();
                        td.style.border = '1px solid black';
                        td.style.textAlign = "center";
                        console.log(j);
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
                                td.appendChild(document.createTextNode(`Информационно-удостоверяющий лист`));
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
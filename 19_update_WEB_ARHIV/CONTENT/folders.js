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
var numFolders;
var listFolders;
let nameFoldersList = new XMLHttpRequest();

nameFoldersList.open('get', 'content_for_web/nameFoldersList', false);
nameFoldersList.onreadystatechange = createTable;
nameFoldersList.send(null);

function createTable() {

    if (nameFoldersList.readyState === 4) {
        listFolders = nameFoldersList.responseText.toString().split('\n');
        numFolders = listFolders.length - 1;
        console.log(numFolders);
    }

    const body = document.body;
    tbl = document.getElementById('table');
    tbl.style.margin = 'auto';
    // tbl.style.width = '400px';

    tbl.style.border = '1px solid black';

    for (let i = 0; i <= numFolders; i++)
    {
        const tr = tbl.insertRow();
        for (let j = 0; j < 2; j++)
        {
            if (i === 0)
            { // Шапка таблицы
                const td = tr.insertCell();
                tr.style.height = '80px';
                td.style.border = '1px solid black';
                td.style.fontWeight = 'bold';
                td.style.fontFamily = 'Arial';
                td.style.fontSize = '20px';
                td.style.textAlign = "center";
                td.style.fontStyle = "italic";
                td.style.padding = '10px';

                switch (j)
                {
                    case 0:
                        // td.style.width = '40px';
                        td.appendChild(document.createTextNode(" Номер "));
                        break;
                    case 1:
                        td.style.paddingLeft = '100px';
                        td.style.paddingRight = '100px';
                        td.appendChild(document.createTextNode(`ИЗДЕЛИЕ`));
                        break;
                }
            } // Шапка таблицы  if (i == 0)
            if (i !== 0)
            { //Остальная таблица
                const td = tr.insertCell();
                td.style.border = '1px solid black';
                td.style.padding = '10px';
                td.style.fontSize = '18px';
                switch (j)
                {
                    case 0:
                        td.appendChild(document.createTextNode(i));
                        td.style.fontWeight = 'bold';
                        td.style.textAlign = "center";
                        // td.style.width = '40px';
                        break;
                    case 1:
                        td.style.fontWeight = 'bold';
                        td.style.width = '200px';
                        td.style.textAlign = "left";
                        let oboznIkodDoc = document.createElement('a');
                        let linkObozn = document.createTextNode(listFolders[i-1]);
                        oboznIkodDoc.style.textDecoration = "none";
                        oboznIkodDoc.appendChild(linkObozn);
                        oboznIkodDoc.title = listFolders[i-1];
                        // oboznIkodDoc.href = 'content_for_web/' + `${i}` + '/' + `${rowData[0]}` + '.PDF';
                        td.appendChild(oboznIkodDoc);
                        //Загрузка контента как файла
                        //download('content_for_web/'+`${i}`+'/' + `${rowData[0]}` + '.PDF',`${rowData[0]}`+ '.pdf');
                        break;
                }
            }//i>0

        } //Заполнение столбцов  for (let j = 0; j < 2; j++)

    } //Строки
    body.appendChild(tbl);

} //function createTable()




// console.log ("Done");

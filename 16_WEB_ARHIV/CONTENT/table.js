
function tableCreate() {
    const body = document.body,
          // tbl = document.createElement('table');
          tbl = document.getElementById('table');
  
    tbl.style.width = '1800px';
    tbl.style.border = '1px solid black';
    for (let i = 0; i < 100; i++) {
      const tr = tbl.insertRow();
      for (let j = 0; j < 8; j++) {
        // if (i === 1 && j === 0) {
        //   break;
        // } else 
        if (i==0)
        {
          const td = tr.insertCell();
          td.style.border = '1px solid black';
          td.style.fontWeight = 'bold';
          td.style.textAlign="center"; 
          td.style.fontStyle = "italic";
          switch(j){
          case 1: td.appendChild(document.createTextNode(`Обозначение и код документа`));break;
          case 2: td.appendChild(document.createTextNode(`Наименование изделия`));break;
          case 3: td.appendChild(document.createTextNode(`Наименование документа`));break;
          case 4: td.appendChild(document.createTextNode(`Номер изменения`));break;
          case 5: td.appendChild(document.createTextNode(`Дата извещения об изменении`));break;
          case 6: td.appendChild(document.createTextNode(`Значение контрольной суммы подлинника`));break;
          case 7: td.appendChild(document.createTextNode(`Информационно-удостоверяющий лист`));break;    
          case 0: td.appendChild(document.createTextNode(`Порядковый номер`));break;               
          }
        }
        if (i!=0)
        {
          if (j==0)
          {
            const td = tr.insertCell();
            td.appendChild(document.createTextNode(i));
            td.style.border = '1px solid black';
            td.style.textAlign="center";
          }
          else
          {
            const td = tr.insertCell();
            td.appendChild(document.createTextNode(`Cell I${i}/J${j}`));
            td.style.border = '1px solid black';
            td.style.textAlign="center";
          }


        }
      }
    }
    body.appendChild(tbl);
  }
  tableCreate();


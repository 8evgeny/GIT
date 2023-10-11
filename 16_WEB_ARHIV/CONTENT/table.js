
function tableCreate() {
    const body = document.body,
          // tbl = document.createElement('table2');
          tbl = document.getElementById('table');
          
    tbl.style.width = '1200px';
    tbl.style.border = '1px solid black';
    for (let i = 0; i < 100; i++) {
      const tr = tbl.insertRow();
      for (let j = 0; j < 7; j++) {
        // if (i === 1 && j === 0) {
        //   break;
        // } else 
        if (i==0)
        {
          const td = tr.insertCell();
          td.style.border = '1px solid black';
          switch(j){
          case 0: td.appendChild(document.createTextNode(`1`));break;
          case 1: td.appendChild(document.createTextNode(`2`));break;
          case 2: td.appendChild(document.createTextNode(`3`));break;
          case 3: td.appendChild(document.createTextNode(`4`));break;
          case 4: td.appendChild(document.createTextNode(`5`));break;
          case 5: td.appendChild(document.createTextNode(`6`));break;
          case 6: td.appendChild(document.createTextNode(`7`));break;                   
          }
        }
        else
        {
          const td = tr.insertCell();
          td.appendChild(document.createTextNode(`Cell I${i}/J${j}`));
          td.style.border = '1px solid black';
        }
      }
    }
    body.appendChild(tbl);
  }
  tableCreate();


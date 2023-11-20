 var reader = new XMLHttpRequest() || new ActiveXObject('MSXML2.XMLHTTP');
     function loadFile(path) {
        reader.open('get', path, true); 
        reader.onreadystatechange = displayContents;
        reader.send(null);
    }
    function displayContents() {

        if(reader.readyState==4) {
            var el = document.getElementById('main');
            let dateTime = reader.responseText;
            el.innerHTML = 'АРХИВ КОНСТРУКТОРСКОЙ ДОКУМЕНТАЦИИ НИОКР   ' + dateTime;
        }
    }


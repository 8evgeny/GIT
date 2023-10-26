 var reader = new XMLHttpRequest() || new ActiveXObject('MSXML2.XMLHTTP');
     function loadFile(path) {
        reader.open('get', path, true); 
        reader.onreadystatechange = displayContents;
        reader.send(null);
    }
    function displayContents() {

        if(reader.readyState==4) {
            var el = document.getElementById('main');
            el.innerHTML = reader.responseText;
        }
    }


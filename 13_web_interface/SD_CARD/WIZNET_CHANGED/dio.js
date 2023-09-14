function DioCallback(o)
{
    var pin = o.dio_p;
    $('txtdio_s'+pin).value=o.dio_s;
    $('txtdio_d'+pin).value=o.dio_d;
}
function getDio(o) 
{
    var p=o.attributes['pin'].value;
    var oUpdate;
    oUpdate=new AJAX('get_dio'+p+'.cgi',function(t){try{eval(t);}catch(e){alert(e);}});
    oUpdate.doGet();
}
function setDiostate(o)
{
    var p=o.attributes['pin'].value;
    /*var v=$('txtdio_s'+p).value;*/
    var v=o.attributes['s'].value;
    dout=new AJAX('set_diostate.cgi',function(t){try{eval(t);}catch(e){alert(e);}});
    dout.doPost('pin='+p+'&val='+v);
}
function setDiodir(o)
{
    var p=o.attributes['pin'].value;
    /*var v=$('txtdio_d'+p).value;*/
    var v=o.attributes['d'].value;
    dout=new AJAX('set_diodir.cgi', function(t){try{eval(t);}catch(e){alert(e);}});
    dout.doPost('pin='+p+'&val='+v);
}

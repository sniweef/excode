function fucRet()
{
    return ;
}
function myFunction()
{
    var int_var = 1;
    var str_var = "test";
    var sum_var = int_var + str_var;
    var uninit_var;
    var float_var = 2.2;
    var null_var = null;
    var num_obj = new Number;
    //document.getElementById("demo").innerHTML="My First JavaScript Function";
    //document.getElementById("demo").innerHTML=float_var+int_var;
    //document.getElementById("demo").innerHTML=null_var;
    document.getElementById("demo").innerHTML=num_obj;
    //document.getElementById("demo").innerHTML=sum_var;
    //document.getElementById("demo").innerHTML=uninit_var;
    //document.getElementById("content").innerHTML="Excellent~"
    document.getElementById("content").innerHTML=fucRet();
}

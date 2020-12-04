
function modulo(a, b) 
{
  return a - Math.floor(a/b)*b;
}

function ToUint32(x) 
{
  return modulo(Number(x), Math.pow(2, 32));
  // return x*100;
}

Application.print("---- Script readsourcedata.js");

var data = Application.readSourceData("qtslavetcp", "hreg0");

var data_string = "";

for(var i = 0; i < data.length; i++)
{
  data_string += String.fromCharCode(data[i]);
}

// Application.print("data = " + data.join(" "));
// Application.print("data = " + data_string);

for(var i = 0; i < data.length; i++)
{
  // Application.print("data = " + data.join(" "));
  // Application.print("data[" + i + "] to Uint32 = " + ToUint32(data[i]));
  Application.print("data[" + i + "] to Uint32 = " + ToUint32(data[i]).toString(16));
}
var reg = 0;

for(var i = 0; i < data.length; i++)
{

  var val = ToUint32(data[i]);
  Application.print("i = " + i);
  Application.print("val = " + val.toString(16));
  val = ToUint32(val << (8 * i));
  reg = ToUint32(reg | val);
  // Application.print("data[" + i + "] = " + data[i]);
  // reg = reg | (data[i] << (i * 8));
  // Application.print("reg[" + i + "] = " + Math.abs(reg).toString(2));
}

Application.print("reg = " + ToUint32(reg).toString(16));





function modulo(a, b) 
{
  return a - Math.floor(a/b)*b;
}

function ToUint32(x) 
{
  return modulo(Number(x), Math.pow(2, 32));
  // return x*100;
}

Application.debug("---- Script readsourcedata.js");

var data = Application.readData("qtslavetcp", "hreg0");

var data_string = "";

for(var i = 0; i < data.length; i++)
{
  data_string += String.fromCharCode(data[i]);
}

// Application.debug("data = " + data.join(" "));
// Application.debug("data = " + data_string);

for(var i = 0; i < data.length; i++)
{
  // Application.debug("data = " + data.join(" "));
  // Application.debug("data[" + i + "] to Uint32 = " + ToUint32(data[i]));
  Application.debug("data[" + i + "] to Uint32 = " + ToUint32(data[i]).toString(16));
}
var reg = 0;

for(var i = 0; i < data.length; i++)
{

  var val = ToUint32(data[i]);
  Application.debug("i = " + i);
  Application.debug("val = " + val.toString(16));
  val = ToUint32(val << (8 * i));
  reg = ToUint32(reg | val);
  // Application.debug("data[" + i + "] = " + data[i]);
  // reg = reg | (data[i] << (i * 8));
  // Application.debug("reg[" + i + "] = " + Math.abs(reg).toString(2));
}

Application.debug("reg = " + ToUint32(reg).toString(16));




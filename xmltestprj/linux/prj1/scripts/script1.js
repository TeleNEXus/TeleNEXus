Application.print("------Script 1");

// var retstr = new String;
var ba = Application.testGetByteArray();
Application.print("Byte array = " + ba.join(""));
Application.print("Byte array size = " + ba.length);
var data = 0;
for(var i = 0; i < ba.length; i++)
{
  Application.print("pass = " + i);
  var val = Number(ba[i]);
  Application.print("pass val = " + (val));
  val = val << (8 * i);
  Application.print("pass val shift = " + val.toString(16));
  data = data | val;
  Application.print("pass data = " + Number(data).toString(16));
  // data = data | (ba[i] << (8 * i));
}
// data = 0x5f;
// data--;
Application.print("data = " + data.toString(2));




// // str = new String;
// var res = 0;
// for(i = 0; i < 3; i++)
// {
//   // str = i;
//   res += Math.sqrt(2)/2.5;
// }
// Application.print("----------------Script 1 cycle res = " + res);
// var prj_path = Application.getProjectPath();
// Application.print("Script 1 " + prj_path);
//
// var res = 0;
// for(i = 0; i < 3000000; i++)
// {
//   // str = i;
//   res += Math.sqrt(30000)/1.5;
// }




// var data_id = "{ request data script 1 }";
// var sourceData = Application.readSourceData(data_id);
// Application.print("---------------> " + sourceData + sourceData + "res = " + res);





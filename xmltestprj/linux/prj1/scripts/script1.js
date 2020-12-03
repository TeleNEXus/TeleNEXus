Application.print("------Script 1");
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
var res = 0;
for(i = 0; i < 3000000; i++)
{
  // str = i;
  res += Math.sqrt(30000)/1.5;
}
var data_id = "{ request data script 1 }";
var sourceData = Application.readSourceData(data_id);
Application.print("---------------> " + sourceData + sourceData + "res = " + res);

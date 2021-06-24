( function ()
  {
    console.debug(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Start Import Module");
  }
)();

var localObject = { field1 : "local 1 meaning", field2 : "local 2 meaning"};
// export var importObject = { field1 : "field 1 meaning", field2 : "field 2 meaning"};
export function testImportDebug(debug_str)
{
  DebugOut(debug_str);
  DebugOut("localObject: field 1 = " + localObject.field1 + "; field 2 = " + localObject.field2 + ";");
}

( function ()
  {
    console.debug("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<End Import Module");
  }
)();

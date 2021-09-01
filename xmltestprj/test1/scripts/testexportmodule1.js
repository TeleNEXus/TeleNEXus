( function ()
  {
    console.debug("\tStart Import Module 'testexportmodule1.js'");
  }
)();

var localObject = { field1 : "local 1 meaning", field2 : "local 2 meaning"};
// export var importObject = { field1 : "field 1 meaning", field2 : "field 2 meaning"};
export function testImportDebug(debug_str)
{
  console.debug(debug_str);
  console.debug("localObject: field 1 = " + localObject.field1 + "; field 2 = " + localObject.field2 + ";");
}

( function ()
  {
    console.debug("\tEnd Import Module 'testexportmodule1.js'");
  }
)();

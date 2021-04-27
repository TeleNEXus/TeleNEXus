console.debug(">>>>>>>>>>>>>>>>>>>>>>Begint test export module script");

var allAttributes = Object.keys(Attributes);

//show attributes
( function ()
  {
    for(var i = 0; i < allAttributes.length; i++)
    {
      console.debug("Attribute " + allAttributes[i] + " = " + Attributes[allAttributes[i]]);
    }
  }
)()

try
{
  // ImportModule("scripts/testexportmodule1.js", "testImportDebug");
  // ImportModule("scripts/testexportmodule1.js", "importObject");
  ImportModule("scripts/testexportmodule1.js");
}
catch(error)
{
  console.debug(error);
}

(
  function Main(){

    testImportDebug("---------------Test Import Debug");
    console.debug("Test import object importObject.field1 = " + importObject.field1);
    console.debug("Test import object importObject.field2 = " + importObject.field2);

    try
    {
    console.debug("Test import object localObject.field1 = " + localObject.field1);
    console.debug("Test import object localObject.field2 = " + localObject.field2);
    }
    catch(error)
    {
      console.debug(error);
    }
  }
)();

console.debug(">>>>>>>>>>>>>>>>>>>>>>End test export module script");

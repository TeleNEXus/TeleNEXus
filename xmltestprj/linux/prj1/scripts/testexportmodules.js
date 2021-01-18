DebugOut(">>>>>>>>>>>>>>>>>>>>>>Begint test export module script");

var allAttributes = Object.keys(Attributes);

function showAttributes(){
  for(var i = 0; i < allAttributes.length; i++){
    DebugOut("Attribute " + allAttributes[i] + " = " + Attributes[allAttributes[i]]);
  }
};

showAttributes();

var executeCounter = 0;

// var exportFlag = ExportModule("/home/serg/pprj/tnex/xmltestprj/linux/prj1/scripts/testexportmodule1.js");
var exportFlag = ExportModule("scripts/testexportmodule1.js");

function Main(){
  // DebugOut("Execute Test Attributes Script N = " + executeCounter);
  testExportDebug("Execute test export debug out N = " + executeCounter);
  DebugOut("DebugOut Main");
  executeCounter++;
  showAttributes();
}

DebugOut(">>>>>>>>>>>>>>>>>>>>>>End test export module script");

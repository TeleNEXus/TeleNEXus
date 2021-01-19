console.debug(">>>>>>>>>>>>>>>>>>>>>>Begint test export module script");

var allAttributes = Object.keys(Attributes);

function showAttributes(){
  for(var i = 0; i < allAttributes.length; i++){
    console.debug("Attribute " + allAttributes[i] + " = " + Attributes[allAttributes[i]]);
  }
};

showAttributes();

var executeCounter = 0;

// var exportFlag = ExportModule("/home/serg/pprj/tnex/xmltestprj/linux/prj1/scripts/testexportmodule1.js");
var exportFlag = ExportModule("scripts/testexportmodule1.js");

function Main(){
  console.debug("Execute Test Attributes Script N = " + executeCounter);
  executeCounter++;
  ExJs("asdfasdf");
  ExJs(function(arg1, arg2){
    console.debug('callback function arg1 = ' + arg1);
    console.debug('callback function arg2 = ' + arg2);
  });
  // showAttributes();
}

console.debug(">>>>>>>>>>>>>>>>>>>>>>End test export module script");

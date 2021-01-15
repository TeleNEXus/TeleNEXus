DebugOut(">>>>>>>>>>>>>>>>>>>>>>Begin Test Attributes Script");

var allAttributes = Object.keys(Attributes);

function showAttributes(){
  for(var i = 0; i < allAttributes.length; i++){
    DebugOut("Attribute " + allAttributes[i] + " = " + Attributes[allAttributes[i]]);
  }
};

showAttributes();

var executeCounter = 0;

function Main(){
  DebugOut("Execute Test Attributes Script N = " + executeCounter);
  executeCounter++;
  showAttributes();
}

DebugOut(">>>>>>>>>>>>>>>>>>>>>>End Test Attributes Script");

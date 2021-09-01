console.debug(">>>>>>>>>>>>>>>>>>>>>>Begin evaluage Test Attributes Script");

var allAttributes = Object.keys(Attributes);

function showAttributes(){
  for(var i = 0; i < allAttributes.length; i++){
    console.debug("Attribute " + allAttributes[i] + " = " + Attributes[allAttributes[i]]);
  }
};

showAttributes();

var executeCounter = 0;

function Main(){
  console.debug("Execute Test Attributes Script N = " + executeCounter);
  executeCounter++;
  showAttributes();
}

console.debug(">>>>>>>>>>>>>>>>>>>>>>End evaluate Test Attributes Script");

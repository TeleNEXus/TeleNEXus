
DebugOut(">>>>>>>>>>>>>>>>>>>>>>Begin Test Attributes Script");

var allAttributes = Object.keys(Attributes);
(function(){
  for(var i = 0; i < allAttributes.length; i++){
    DebugOut("Attribute " + allAttributes[i] + " = " + Attributes[allAttributes[i]]);
  }
})();

function Main(){
  // DebugOut("Execute Test Attributes Sctipt");
DebugOut(">>>>>>>>>>>>>>>>>>>>>>End Test Attributes Script");
}

DebugOut(">>>>>>>>>>>>>>>>>>>>>>End Test Attributes Script");

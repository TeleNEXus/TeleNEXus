DebugOut("Start script: scaledInt32");
DebugOut("All attributes:");

var attributes_keys = Object.keys(Attributes);

for(var i = 0; i < attributes_keys.length; i++) {

  DebugOut("  Attributes[ " 
    + attributes_keys[i] 
    + "\t] = '" 
    + Attributes[ attributes_keys[i] ] 
    + "'");

}

var valrange = {
  min : parseInt(Attributes.min),
  max : parseInt(Attributes.max)
}

DebugOut("valrange.min = " + valrange.min);
DebugOut("valrange.max = " + valrange.max);

function Validate(_val){
  DebugOut("Validate value = " + _val);
  var sign = 1;
  if(typeof _val !== "string") {
    return Invalid;}

  if(_val.length === 0) return Intermediate;
  
  if(_val === "-") return Intermediate;

  var data = parseInt(_val);

  if(isNaN(data)) {
    DebugOut("Validate data is NaN!");
    return Invalid;
  }
  DebugOut("Validate data = " + data);
  if((data > valrange.max)||(data < valrange.min)) return Invalid;
  return Acceptable;
}

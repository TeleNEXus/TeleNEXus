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

function initValrange(){
  var minval = -2147483648;
  var maxval = 2147483647;
  var buff = parseInt(Attributes.min);
  if(!isNaN(buff)){
    minval = buff;
  }
  buff = parseInt(Attributes.max);
  if(!isNaN(buff)){
    maxval = buff;
  }
  return {min : minval, max : maxval};
}

var valrange = initValrange();

DebugOut("valrange.min = " + valrange.min);
DebugOut("valrange.max = " + valrange.max);

function initScale(){
  if(Attributes.format === undefined) {
    DebugOut("scaledInt32 format is undefined");
  }
  else {
    DebugOut("scaledInt32 format = " + Attributes.format);
  }

  // re = new RegExp("x*");
  var re = /\.x*/;
  var regExp = new RegExp("(\..+)$");

  if(!regExp.test(Attributes.format)) {
    DebugOut("scaledInt32 wrong format string.");
  }
  else {
    DebugOut("scaledInt32 format string is correct.");
  }
  return 0
}

var scale = initScale();

//------------------------------------------------------------------------------Validate
function Validate(_val){

  DebugOut("Validate value = " + _val);
  var sign = 1;
  if(typeof _val !== "string") {
    return Invalid;
  }

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

//------------------------------------------------------------------------------ToString
function arrayToInt(_data) {

  var i = 0;
  var res = 0;
  for(i = 0; i < _data.length; i++)
  {
    res |= (_data[i]&0xff) << (8*i);
  }
  return res;
}

//------------------------------------------------------------------------------ToString
function ToString(_data) {

  DebugOut("ToString data = " + _data);
  var res_int = arrayToInt(_data);
  DebugOut("arrayToInt() = " + res_int);
  return res_int.toString();
}

//------------------------------------------------------------------------------ToBytes
function ToBytes(_str) {

  var array = [0,0,0,0];
  var int_data = parseInt(_str);
  DebugOut("======================================================ToBytes");
  DebugOut("int_data = " + int_data);
  if(isNaN(int_data)) return [];
  for (var i = 0; i < 4; i++)
  {
    array[i] = 0x000000ff & (int_data >>> (8*i));
  }
  DebugOut("------------------------------ToBytes" + array);
  return array;
}


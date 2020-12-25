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
  var min = -2147483648;
  var max = 2147483647;

  var buff = parseFloat(Attributes.min);
  if(!isNaN(buff)){
    min = buff;
  }
  buff = parseFloat(Attributes.max);
  if(!isNaN(buff)){
    max = buff;
  }
  return {min : min, max : max};
}

var valrange = initValrange();

DebugOut("valrange.min = " + valrange.min);
DebugOut("valrange.max = " + valrange.max);

function initFormat(){

  var format = {
    intsize   : 0,
    fractsize : 0,
    scale     : 1,
    pow       : 0
  }

  if(Attributes.format === undefined) {
    return format;
  }

  var regExp = /\..+$/;

  format.fractsize = Attributes.format.search(regExp);

  if(format.fractsize < 0) {
    format.fractsize = 0;
  }
  else {
    format.fractsize = Attributes.format.length - 1 - format.fractsize;
    if(format.fractsize < 0) format.fractsize = 0;
  }

  format.intsize = Attributes.format.length - 1 - format.fractsize;
  if(format.intsize < 0) format.intsize = 0;

  format.scale = Math.pow(10, (-1)*format.fractsize);
  format.pow = format.fractsize;

  DebugOut("scaledInt32 intsize = " + format.intsize);
  DebugOut("scaledInt32 fractsize = " + format.fractsize);
  DebugOut("scaledInt32 scale = " + format.scale);

  return format;
}

var gvFormat = initFormat();

//------------------------------------------------------------------------------Validate
function Validate(_val){

  DebugOut("Validate value = " + _val);
  var sign = 1;
  if(typeof _val !== "string") {
    return Invalid;
  }

  if(_val.length === 0) return Intermediate;

  if((_val === "-") || (_val === ".")) return Intermediate;

  if(gvFormat.scale === 1) {
    if(!RegExp(/^-{0,1}[0-9]*$/).test(_val)) return Invalid;
    var data = parseInt(_val);
  }
  else {
    DebugOut("gvFormat.scale = " + gvFormat.scale);
    if(!RegExp(/^-{0,1}([0-9]*)(\.{0,1})([0-9]*$)/).test(_val)) return Invalid;
    var data = parseFloat(_val);
  }

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

function formatting(_input){

  var str = _input.toFixed(gvFormat.fractsize);

  var len = gvFormat.intsize + gvFormat.fractsize + 1;

  var minus = "";

  if(str.charAt(0) == "-") {
    minus = "-";
  }

  str = str.replace(/(^-){1,1}/,"");
  if(str.length < len)
  {
    var addzero = "";
    for (var i = 0; i < (len - str.length); i++)
    {
      addzero += "0";
    }
    str = addzero + str;
  }
  
  return minus + str;
}

//------------------------------------------------------------------------------ToString
function ToString(_data) {

  DebugOut("ToString data = " + _data);

  var res_int = arrayToInt(_data);

  DebugOut("arrayToInt() = " + res_int);

  res_int *= gvFormat.scale;

  return formatting(res_int);
}

function parseToNum(_str)
{
  if(gvFormat.scale === 1) return parseInt(_str);
  return parseFloat(_str) * Math.pow(10, gvFormat.pow);
}

//------------------------------------------------------------------------------ToBytes
function ToBytes(_str) {

  var array = [0,0,0,0];
  var num_data = parseToNum(_str);

  DebugOut("======================================================ToBytes");
  DebugOut("int_data = " + num_data);
  if(isNaN(num_data)) return [];
  for (var i = 0; i < 4; i++)
  {
    array[i] = 0x000000ff & (num_data >>> (8*i));
  }
  DebugOut("------------------------------ToBytes" + array);
  return array;
}


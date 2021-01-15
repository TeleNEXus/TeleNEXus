/* 
 * Copyright (C) 2020 Sergey S. Kuzmenko
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with This program.  If not, see <https://www.gnu.org/licenses/>.
 */

/*------------------------------------------------------------------------------
 * Скрипт форматирования данных типа integer 32 bit с использованием 
 * заданного шаблона форматирования.
 *
 *  Передаваемые атрибуты:
 * -------------------------------------
 *  id      - идентификатор скрипта.
 *  file    - файл скрипта.
 *  min     - минимальное значение величины.
 *  max     - максимальное значение величины.
 *  format  - строка шаблона форматирования в виде "iii.fff" 
 *            (i - цедая часть, f - дробная часть).
 */


//------------------------------------------------------------------------------
DebugOut(">>>>>>>>>>>>>>>>>>>>>>>>>>>> Formatter Script " + Attributes.id);
DebugOut(">>>>>>>>>>>>>>>>>>>>>>>>>>>> Formatter Script File" + Attributes.file);

//------------------------------------------------------------------------------
//Debug all DOM attributes.
var attributes_keys = Object.keys(Attributes);

for(var i = 0; i < attributes_keys.length; i++) {

  DebugOut("  Attributes[ " 
    + attributes_keys[i] 
    + "\t] = '" 
    + Attributes[ attributes_keys[i] ] 
    + "'");
}

//------------------------------------------------------------------------------
//Init range of value.
var valrange = ( function() { 
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
})();

//------------------------------------------------------------------------------
//Init data format.
var gvFormat = (function () {

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

  return format;
})();


//------------------------------------------------------------------------------arrayToString
function arrayToInt(_data) {

  var i = 0;
  var res = 0;
  for(i = 0; i < _data.length; i++)
  {
    res |= (_data[i]&0xff) << (8*i);
  }
  return res;
}

//------------------------------------------------------------------------------formatting
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

//==============================================================================Validate
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

//==============================================================================ToString
function ToString(_data) {

  var res_int = arrayToInt(_data) * gvFormat.scale;
  return formatting(res_int);
}

//==============================================================================ToBytes
function ToBytes(_str) {

  var array = [0,0,0,0];

  //get numeric data.
  var num_data = (function(){
      if(gvFormat.scale === 1) return parseInt(_str);
      return parseFloat(_str) * Math.pow(10, gvFormat.pow);
    })();

  if(isNaN(num_data)) return [];
  for (var i = 0; i < 4; i++)
  {
    array[i] = 0x000000ff & (num_data >>> (8*i));
  }
  return array;
}

//==============================================================================Fitting
function Fitting(_str)
{
  return ToString(ToBytes(_str));
}


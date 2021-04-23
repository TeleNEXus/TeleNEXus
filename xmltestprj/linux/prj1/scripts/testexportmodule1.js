(function (){
  DebugOut("+++++Start Import Module");
})();


var importObject = { field1 : "field 1 meaning", field2 : "field 2 meaning"};

function testImportDebug(debug_str){
  DebugOut(debug_str);
}

(function (){
  console.debug("+++++End Import Module");
})();

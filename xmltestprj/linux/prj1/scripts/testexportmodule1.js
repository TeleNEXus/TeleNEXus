(function (){
  DebugOut("+++++Start Export Module");
})();

function testExportDebug(debug_str){
  DebugOut(debug_str);
}

(function (){
  console.debug("+++++End Export Module");
})();

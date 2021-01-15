
var sourceId = "qtslavetcp";
var dataId = "hreg1";
var read_reg = 0;

//==============================================================================
function sleep(milliseconds) {
  var date = Date.now();
  var currentDate = 0;
  do {
    currentDate = Date.now();
  } while (currentDate - date < milliseconds);
}

//==============================================================================Main
function Main(){
  var read_data = DataSourceRead(sourceId, dataId);
  DebugOut("Read Data = " + read_data);
  for(var i = 0; i < read_data.length; i++)
  {
    read_reg = (read_reg | (read_data[i] << (8 * i)));
  }

  read_reg++;
  read_reg &= 0xffff;

  var write_data = [read_reg & 0x00ff, read_reg >>> 8];
  var write_size = DataSourceWrite(sourceId, dataId, write_data);
}

DebugOut("End Script");



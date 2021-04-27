function asf(){Application.debug("Timeout==============1"); }

//setTimeout(asf(), 100);
// setInterval(asf(), 100);

function sleep(milliseconds) {
  var date = Date.now();
  var currentDate = 0;
  do {
    currentDate = Date.now();
  } while (currentDate - date < milliseconds);
}

// sleep(2000);
//
Application.debug("---- Script writedatatosource.js");
// var sourceId = "qtslavetcp";
// var dataId = "hreg1";
var sourceId = "localsource";
var dataId = "bytes_uint32";
var read_reg = 0;

var read_data = Application.readData(sourceId, dataId);
Application.debug("Read Data = " + read_data);
for(var i = 0; i < read_data.length; i++)
{
  read_reg = (read_reg | (read_data[i] << (8 * i)));
}

Application.debug("Register = " + read_reg);
read_reg++;
read_reg &= 0xffffffff;


var write_data = [read_reg & 0x00ff, read_reg >>> 8, read_reg >>> 8*2, read_reg >>> 8*3];
var write_size = Application.writeData(sourceId, dataId, write_data);

Application.debug("Write Size = " + write_size);

Application.debug("End Script");



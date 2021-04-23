console.debug(">>>>>>>>>>>>>>>>>>>> Begin start test interprc.interaction");

var numArr = [
  1, 2, 3, 4, 5, 6, 7, 8, 9,
  10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
  20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
  30, 31, 32, 33, 34, 35, 36, 37, 38, 39,
  40, 41, 42, 43, 44, 45, 46, 47, 48, 49,
  50, 51, 52, 53, 54, 55, 56, 57, 58, 59,
  60, 61, 62, 63, 64, 65, 66, 67, 68, 69,
  70, 71, 72, 73, 74, 75, 76, 77, 78, 79,
  80, 81, 82, 83, 84, 85, 86, 87, 88, 89,
  90, 91, 92, 93, 94, 95, 96, 97, 98, 99,
  100, 101, 102, 103, 104, 105, 106, 107, 108, 109,
  110, 111, 112, 113, 114, 115, 116, 117, 118, 119,
  120, 121, 122, 123, 124, 125, 126, 127, 128, 129,
  130, 131, 132, 133, 134, 135, 136, 137, 138, 139,
  140, 141, 142, 143, 144, 145, 146, 147, 148, 149,
  150, 151, 152, 153, 154, 155, 156, 157, 158, 159,
  160, 161, 162, 163, 164, 165, 166, 167, 168, 169,
  170, 171, 172, 173, 174, 175, 176, 177, 178, 179,
  180, 181, 182, 183, 184, 185, 186, 187, 188, 189,
  190, 191, 192, 193, 194, 195, 196, 197, 198, 199,
  200, 201, 202, 203, 204, 205, 206, 207, 208, 209,
  210, 211, 212, 213, 214, 215, 216, 217, 218, 219,
  220, 221, 222, 223, 224, 225, 226, 227, 228, 229,
  230, 231, 232, 233, 234, 235, 236, 237, 238, 239,
  240, 241, 242, 243, 244, 245, 246, 247, 248, 249,
  250, 251, 252, 253, 254, 255, 0
];

function byteArrayToString(arr)
{
  var str = new String("");
  for(let i=0; i < arr.length; i++)
    str += String.fromCharCode(arr[i]);
  return str;
}

function stringToByteArray(str)
{
  let retArr = str.split("");
  for(let i=0; i < retArr.length; i++)
  {
    retArr[i] = retArr[i].charCodeAt(0);
  }
  return retArr;
}

function transferData(process)
{
  var data_count;

  try
  {
    var data_count = process.write(stringToByteArray("Hello World!!!"));
  }
  catch(error)
  {
    console.warn("In " + error.fileName + ": " + error.lineNumber + ": " + error.message);
  }

  console.debug("Writed data count = " + data_count);

  try
  {
    process.waitForBytesWritten();
    console.debug("Process bytes is writen");
  }
  catch(error)
  {
    console.warn("In " + error.fileName + ": " + error.lineNumber + ": " + error.message);
  }


  // process.closeWriteChannel();


  var read_data;

  try
  {
    process.waitForReadyRead(1000);
    console.debug("Process ready read");
    read_data = process.readAll();
    console.debug("Readed data = " + byteArrayToString(read_data));
  }
  catch(error)
  {
    console.warn("In " + error.fileName + ": " + error.lineNumber + ": " + error.message);
  }
}

function Main() {

  let process = NewProcess();
  process.start("interprocess/__build/interprocess");

  try{
    process.waitForStarted(1000);
    console.debug("Process start normal");
  }catch(error){
    console.warn("In " + error.fileName + ": " + error.lineNumber + ": " + error.message);
  }

  console.debug("Start transfer data");
  var counter = 0;
  while(true)
  {
    console.debug("Next data transfer " + counter);
    transferData(process);

    try{
      process.waitForFinished(500);
      console.debug("Process finished normal");
      break;
    }catch(error){
      // console.warn("In " + error.fileName + ": " + error.lineNumber + ": " + error.message);
    }
    counter++;
  }
};

console.debug("<<<<<<<<<<<<<<<<<<<<<<<< Stop test interprc.interaction");

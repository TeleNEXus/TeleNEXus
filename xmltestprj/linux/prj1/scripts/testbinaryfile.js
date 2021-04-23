console.debug("<<<<<<<<<<<<<<<<Begin Evaluate Script binaryfile.js");

//test text file write
function testWrite(i){

  console.debug("Test write binary file #" + i);

  var File = NewBinaryFile("temp/test_binary_file_write.bin");

  var data = [0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f];

  try {
    File.open('w');
  }catch(error){
    console.warn("In " + error.fileName + ": " + error.lineNumber + ": open file " 
      + File.fileName() + " error: " + error.message);
  }

  try {
    File.write(data);
  }catch(error){
    console.warn("In " + error.fileName + ": " + error.lineNumber + ": " + error.message);
  }

  File.close();

};


(
  function Main(){

    testWrite(1);
    testWrite(2);

    console.debug("Test read binary file");
    let File = NewBinaryFile('temp/test_binary_file_write.bin');
    try{
      File.open('r');
      console.debug("Read file size = " + File.size());
    }catch(error){
      console.warn("In " + error.fileName + ": " + error.lineNumber + ": open file " 
        + File.fileName() + " error: " + error.message);
    }

    var read_data;

    try{
      File.seek(10);
      read_data = File.read(1);
    console.debug("Read file data = " + read_data);
      File.seek(0);
      read_data = File.read(1);
    console.debug("Read file data = " + read_data);
    }catch(error){
      console.warn("In " + error.fileName + ": " + error.lineNumber + ": open file " 
        + File.fileName() + " error: " + error.message);
    }

    if(File.atEnd()) console.debug("End the file");
    else console.debug("File not at end");

    // console.debug("Read file data = " + read_data);
  }
)();


 

////test text file read
//function read_file(){

//  console.debug("Test text file read");

//  let File = NewTextFile();
//  // let File = NewTextFile('temp/test_text_file_read.txt');
//  // let File = new TextFile('temp/test_text_file_read.txt');
//  File.setFileName('temp/test_text_file_read.txt');
//  File.open('r');

//  console.debug("\nText file read all 1");
//  console.debug(File.readAll());
//  console.debug("\nText file read all 2");
//  File.seek(0);
//  console.debug(File.readAll());
  
//  console.debug("\nText file read line 1");
//  File.seek(0);
//  console.debug("read line = " + File.readLine(1));
//  console.debug("read line = " + File.readLine(2));
//  console.debug("read line = " + File.readLine(3));
//  console.debug("read line = " + File.readLine(4));
//  console.debug("read line = " + File.readLine(5));
//  console.debug("read line = " + File.readLine(6));
//  console.debug("read line = " + File.readLine(7));
//  console.debug("read line = " + File.readLine(8));
//  console.debug("read line = " + File.readLine(9));
//  console.debug("read line = " + File.readLine(10));

//  console.debug("\nText file read line 2");
//  File.seek(0);
//  console.debug("read line = " + File.readLine());
//  console.debug("read line = " + File.readLine());
//  console.debug("read line = " + File.readLine());
//  console.debug("read line = " + File.readLine());
//  console.debug("read line = " + File.readLine());
//  console.debug("read line = " + File.readLine());
//  console.debug("read line = " + File.readLine());
//  console.debug("read line = " + File.readLine());
//  console.debug("read line = " + File.readLine());
//  console.debug("read line = " + File.readLine());
//};

//(function(){
//  let File = NewTextFile('temp/test_text_file_write.txt');
//  // File.open('r');
//  File.copy('temp/test_text_file_write1.txt');
//})();

CollectGarbage();
// if(textFile.rename(newFileName)) console.debug("textFile is renamed");
// else console.debug("textFile is not renamed");

// if(textFile.remove()) console.debug("textFile is removed");
// else console.debug("textFile is not removed");

// console.debug("Write file status = " + textFile.write('Test Write String Hello ' + 1 + '!!! \n'));
// // console.debug("Write file status = " + textFile.write("asdfas"));
// console.debug("Write file status = " + textFile.write('Test Write String Hello ' + 2 + '!!! \n'));
// console.debug("Write file status = " + textFile.write('Test Write String Hello ' + 3 + '!!! \n'));
// textFile.close();
// console.debug("Write file status = " + textFile.write('Test Write String Hello ' + 4 + '!!! \n'));

// textFile.close();
// textFile.open('r');
// console.debug('Read all file data \n' + textFile.readAll());

// textFile.close();

console.debug(">>>>>>>>>>>>>>>>End Evaluate Script testtextfile.js");
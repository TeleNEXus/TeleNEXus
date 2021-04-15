console.debug("<<<<<<<<<<<<<<<<Begin Evaluate Script testtextfile.js");
var textFile = new TextFile('temp/text_file3.txt');
var newFileName = 'temp/text_file2.txt';
// var textFile = new TextFile();
console.info("TextFile info: " + textFile);
console.debug("textFile.fileName = " + textFile.fileName());
if(textFile.exists()) console.debug("textFile is exists");
else console.debug("textFile is not exists");

textFile.open('w');
if(textFile.isOpen()) console.debug("textFile is open")
else console.debug("textFile not opet");
let globalvar = "Test let operator";
const constString = "Test const String.";

//test text file read
(function(){

  console.debug("Test text file read");

  var textFile = new TextFile('temp/test_text_file_read.txt');
  textFile.open('r');

  console.debug("\nText file read all 1");
  console.debug(textFile.readAll());
  console.debug("\nText file read all 2");
  textFile.seek(0);
  console.debug(textFile.readAll());
  
  console.debug("\nText file read line 1");
  textFile.seek(0);
  console.debug("read line = " + textFile.readLine(1));
  console.debug("read line = " + textFile.readLine(2));
  console.debug("read line = " + textFile.readLine(3));
  console.debug("read line = " + textFile.readLine(4));
  console.debug("read line = " + textFile.readLine(5));
  console.debug("read line = " + textFile.readLine(6));
  console.debug("read line = " + textFile.readLine(7));
  console.debug("read line = " + textFile.readLine(8));
  console.debug("read line = " + textFile.readLine(9));
  console.debug("read line = " + textFile.readLine(10));

  console.debug("\nText file read line 2");
  textFile.seek(0);
  console.debug("read line = " + textFile.readLine());
  console.debug("read line = " + textFile.readLine());
  console.debug("read line = " + textFile.readLine());
  console.debug("read line = " + textFile.readLine());
  console.debug("read line = " + textFile.readLine());
  console.debug("read line = " + textFile.readLine());
  console.debug("read line = " + textFile.readLine());
  console.debug("read line = " + textFile.readLine());
  console.debug("read line = " + textFile.readLine());
  console.debug("read line = " + textFile.readLine());
})();

//test text file write
(function(){

  console.debug("Test text file write");

  var textFile = new TextFile('temp/test_text_file_write.txt');
  textFile.open('w');
  textFile.write("Test text file write: Hellow World!!! 1");
  textFile.write("Test text file write: Hellow World!!! 2");
  textFile.write("Test text file write: Hellow World!!! 3");
  textFile.write("Test text file write: Hellow World!!! 4");
  textFile.write("Test text file write: Hellow World!!! 5");
  textFile.write("Test text file write: Hellow World!!! 6");

})();

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

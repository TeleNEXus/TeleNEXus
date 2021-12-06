console.debug("<<<<<<<<<<<<<<<<Begin Evaluate Script testtextfile.js");

(function(){
  var textFile = NewTextFile('temp/text_file3.txt');
  var newFileName = 'temp/text_file2.txt';
  // var textFile = new TextFile();
  console.info("TextFile info: " + textFile);
  console.debug("textFile.fileName = " + textFile.fileName());
  if(textFile.exists()) console.debug("textFile is exists");
  else console.debug("textFile is not exists");

  textFile.open('w');
  if(textFile.isOpen()) console.debug("textFile is open")
  else console.debug("textFile not opet");
})();

//test text file read
function read_file(){

  console.debug("Test text file read");

  let File = NewTextFile();
  // let File = NewTextFile('temp/test_text_file_read.txt');
  // let File = new TextFile('temp/test_text_file_read.txt');
  File.setFileName('temp/test_text_file_write.txt');
  File.open('r');

  console.debug("\nText file read all 1");
  console.debug(File.readAll());
  console.debug("\nText file read all 2");
  File.seek(0);
  console.debug(File.readAll());
  
  console.debug("\nText file read line 1");
  File.seek(0);
  console.debug("read line = " + File.readLine(1));
  console.debug("read line = " + File.readLine(2));
  console.debug("read line = " + File.readLine(3));
  console.debug("read line = " + File.readLine(4));
  console.debug("read line = " + File.readLine(5));
  console.debug("read line = " + File.readLine(6));
  console.debug("read line = " + File.readLine(7));
  console.debug("read line = " + File.readLine(8));
  console.debug("read line = " + File.readLine(9));
  console.debug("read line = " + File.readLine(10));

  console.debug("\nText file read line 2");
  File.seek(0);
  console.debug("read line = " + File.readLine());
  console.debug("read line = " + File.readLine());
  console.debug("read line = " + File.readLine());
  console.debug("read line = " + File.readLine());
  console.debug("read line = " + File.readLine());
  console.debug("read line = " + File.readLine());
  console.debug("read line = " + File.readLine());
  console.debug("read line = " + File.readLine());
  console.debug("read line = " + File.readLine());
  console.debug("read line = " + File.readLine());
};

//test text file write
(function(){

  // read_file();
  // read_file();
  // read_file();
  // read_file();
  // read_file();
  // read_file();
  // read_file();
  console.debug("Test text file write");
  // let File = NewTextFile('~/test_text_file_write.txt');
  // let File = NewTextFile('/home/serg/test_text_file_write.txt');
  let File = NewTextFile('temp/test_text_file_write.txt');
  // let File = new TextFile('temp/test_text_file_write.txt');
  try {
  File.open('w');
  File.setEncoding("Windows-1251");
  }catch(error){
    console.warn("In " + error.fileName + ": " + error.lineNumber + ": open file " 
      + File.fileName() + " error: " + error.message);
  }
  try {
  File.write("Test text file write: Hellow World!!! 1\n");
  File.write("Test text file write: Hellow World!!! 2\n");
  File.write("Test text file write: Hellow World!!! 3\n");
  File.write("Test text file write: Hellow World!!! 4\n");
  File.write("Test text file write: Hellow World!!! 5\n");
  File.write("Test text file write: Hellow World!!! 6\n");

  File.write("Test text file write: Привет мир!!! 1\n");
  File.write("Test text file write: Привет мир!!! 2\n");
  File.write("Test text file write: Привет мир!!! 3\n");
  File.write("Test text file write: Привет мир!!! 4\n");
  File.write("Test text file write: Привет мир!!! 5\n");
  File.write("Test text file write: Привет мир!!! 6\n");
  }catch(error){
    console.warn("In " + error.fileName + ": " + error.lineNumber + ": " + error.message);
  }
})();
// (function(){
//   let File = NewTextFile('temp/test_text_file_write.txt');
//   // File.open('r');
//   File.copy('temp/test_text_file_write1.txt');
// })();

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

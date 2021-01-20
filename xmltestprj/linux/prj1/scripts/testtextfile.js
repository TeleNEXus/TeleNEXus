console.debug("<<<<<<<<<<<<<<<<Begin Evaluate Script testtextfile.js");
var textFile = new TextFile('temp/text_file1.txt');
var newFileName = 'temp/text_file2.txt';
// var textFile = new TextFile();
console.info("TextFile info: " + textFile);
console.debug("textFile.fileName = " + textFile.fileName());
if(textFile.exists()) console.debug("textFile is exists");
else console.debug("textFile is not exists");

textFile.open('a');
if(textFile.isOpen()) console.debug("textFile is open")
else console.debug("textFile not opet");

// if(textFile.rename(newFileName)) console.debug("textFile is renamed");
// else console.debug("textFile is not renamed");

// if(textFile.remove()) console.debug("textFile is removed");
// else console.debug("textFile is not removed");

console.debug("Write file status = " + textFile.write('Test Write String Hello ' + 1 + '!!! \n'));
console.debug("Write file status = " + textFile.write('Test Write String Hello ' + 2 + '!!! \n'));
console.debug("Write file status = " + textFile.write('Test Write String Hello ' + 3 + '!!! \n'));
textFile.close();
console.debug("Write file status = " + textFile.write('Test Write String Hello ' + 4 + '!!! \n'));

textFile.close();
textFile.open('r');
console.debug('Read all file data \n' + textFile.readAll());

textFile.close();

console.debug(">>>>>>>>>>>>>>>>End Evaluate Script testtextfile.js");

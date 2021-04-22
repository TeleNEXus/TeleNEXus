console.debug(">>>>>>>>>>>>>>>>>>>> Begin start test interprc.interaction");

(function () {
  let process = NewProcess();
  process.start("interprocess/__build/interprocess");

  try{
    process.waitForStarted(1000);
    console.debug("Process start normal");
  }catch(error){
    console.warn("In " + error.fileName + ": " + error.lineNumber + ": " + error.message);
  }

  // let wr = [0,1,2,3,4,5];
  // let wr = ["H","E","L","L","O"];
  let wr = Array.from("Hello World");
    let data_count = process.write(wr);
    console.debug("Writed data count = " + data_count);
  // try{
  //   // let data_count = process.write("Hello World!!!");
  // }catch(error){
  //   console.warn("In " + error.fileName + ": " + error.lineNumber + ": " + error.message);
  // }

  try{
    process.waitForBytesWritten();
    console.debug("Process bytes is writen");
  }catch(error){
    console.warn("In " + error.fileName + ": " + error.lineNumber + ": " + error.message);
  }

  process.closeWriteChannel();
 
  let read_data;
  try{
    process.waitForReadyRead(1000);
    console.debug("Process ready read");
    read_data = process.readAll();
    console.debug("Readed data = " + read_data);
  }catch(error){
    console.warn("In " + error.fileName + ": " + error.lineNumber + ": " + error.message);
  }

  try{
    process.waitForFinished(2000);
    console.debug("Process finished normal");
  }catch(error){
    console.warn("In " + error.fileName + ": " + error.lineNumber + ": " + error.message);
  }

})();


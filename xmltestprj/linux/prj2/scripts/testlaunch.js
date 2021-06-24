console.debug("<<<<<<<<<<<<<<<< Begining evaluate the script launch test");

var pass = 0;

ImportModule("scripts/testexportmodule1.js");

function Main()
{
  // console.debug("Test script pass " + pass);
  console.debug("Test script pass " + pass);
  pass++;
}

console.debug(">>>>>>>>>>>>>>>> End evaluate the script launch test");


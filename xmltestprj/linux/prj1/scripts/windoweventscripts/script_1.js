console.debug("<<<<<<<<<<<<<<<< Begin evaluate window event script 1");
var k = 0;
var pass = 0;
function Main()
{
  console.debug("++++++ Main script 1 begin");
  for(let i = 0; i < 100000000; i++)
  {
    k = Math.sqrt(i);
    // console.debug("sqrt(" + i + ") = " + Math.sqrt(i));
  }
  console.debug("++++++ Main script 1 end k = " + k + " pass = " + pass);
  pass++;
}
console.debug(">>>>>>>>>>>>>>>> End evaluate window event script 1");

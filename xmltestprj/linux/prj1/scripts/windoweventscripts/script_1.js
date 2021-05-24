console.debug("<<<<<<<<<<<<<<<< Begin evaluate window event script 1");
var k = 0;
function Main()
{
  console.debug("++++++ Main script 1 begin");
  for(let i = 0; i < 1; i++)
  {
    k = Math.sqrt(i);
    // console.debug("sqrt(" + i + ") = " + Math.sqrt(i));
  }
  console.debug("++++++ Main script 1 end k = " + k);
}
console.debug(">>>>>>>>>>>>>>>> End evaluate window event script 1");

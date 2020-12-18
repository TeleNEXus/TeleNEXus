var counter = 0;
function validate(val)
{
  if(typeof val != "string") {return false;}
  var data = parseInt(val);
  if(isNaN(data))
  {
    Application.debug("function validate() data is NaN");
  }
  Application.debug("function validate() val = " + val + "; data = " + data + ";");
  counter++;
}

Application.debug("Jave Script Evaluate");

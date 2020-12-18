var counter = 0;
var min = -100;
var max = 100;

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
// Application.debug("Jave Script Evaluate EValidateStatus intermediate = " + EValidateStatus.intermediate);
// Application.debug("Jave Script Evaluate EValidateStatus acceptable = " + EValidateStatus.acceptable);
// Application.debug("Jave Script Evaluate EValidateStatus invalid = " + EValidateStatus.invalid);
Application.debug("Jave Script Evaluate EValidateStatus intermediate = " + Application.Intermediate);
Application.debug("Jave Script Evaluate EValidateStatus acceptable = " + Application.Acceptable);
Application.debug("Jave Script Evaluate EValidateStatus invalid = " + Application.Invalid);


var obj1 = { xvalue:0, yvalue:0, zvalue:0};

function validate(val)
{
  var sign = 1;
  

  Application.debug("JS Validator valmin = " + attributes.valmin);
  Application.debug("JS Validator valmax = " + attributes.valmax);

  if(typeof val !== "string") {return Application.Invalid;}

  if(val.length === 0) return Application.Intermediate;

  if(val.charAt(0) === "-"){
    sign = -1;
    // val = val.slice(1, val.length);
    val = val.slice(1);
    Application.debug("JS Validator val = " + val);
  }

  if(val.length === 0) return Application.Intermediate;

  if(val === "0x") return Application.Intermediate;

  var regExp = new RegExp("((^(0x){1,1})([a-fA-F0-9]*)$)|(^([0-9]{1,})$)");

  if(regExp.test(val) === false) {
    Application.debug("JS RegExpt fault.");
    return Application.Invalid;
  }

  var data = parseInt(val);

  Application.debug("JS Validator parse to int = " + sign*data);
  if(isNaN(data)) {
    Application.debug("JS data is NaN!");
    return Application.Invalid;
  }

  if((data > attributes.valmax)||(data < attributes.valmin)) return Application.Invalid;
  return Application.Acceptable;
}

Application.debug("Jave Script Evaluate");

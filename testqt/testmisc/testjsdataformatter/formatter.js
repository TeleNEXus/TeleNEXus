var attributes_keys = Object.keys(Attributes);

DebugOut("All attributes");

for(var i = 0; i < attributes_keys.length; i++)
{
  DebugOut("  Attributes." + attributes_keys[i] + " = " + Attributes[ attributes_keys[i] ]);
}

function validate(val)
{
  var sign = 1;

  DebugOut("JS Validator valmin = " + Attributes.valmin);
  DebugOut("JS Validator valmax = " + Attributes.valmax);

  if(typeof val !== "string") {return Invalid;}

  if(val.length === 0) return Intermediate;

  // var regExp = new RegExp("((^(0x){1,1})([a-fA-F0-9]*)$)|(^([0-9]*)$)");

  // if(regExp.test(val) === false) {
  //   DebugOut("JS RegExpt fault.");
  //   return Invalid;
  // }
  
  if(val === "-") return Intermediate;

  // var data = parseInt(val);
  var data = parseFloat(val);
  // var data = parseDouble(val);

  if(isNaN(data)) {
    DebugOut("JS data is NaN!");
    return Invalid;
  }
  data *= 0.1;
  DebugOut("JS Validator data = " + data);

  if((data > Attributes.valmax)||(data < Attributes.valmin)) return Invalid;
  return Acceptable;
}

DebugOut("Jave Script Evaluate");

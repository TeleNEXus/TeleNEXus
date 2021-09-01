var attributes_keys = Object.keys(Attributes);

Message("All attributes------");
Message("Attrib valmin = " + Attributes.valmin);
var asd = { x:111, y:222};
Message("asd.x = " + asd.x);
Message("asd.y = " + asd.y);

for(var i = 0; i < attributes_keys.length; i++)
{
  Message("  Attributes." + attributes_keys[i] + " = " + Attributes[ attributes_keys[i] ]);
}

function validate(val)
{
  var sign = 1;
  if(typeof val !== "string") {return Invalid;}
  if(val.length === 0) return Intermediate;

  // var regExp = new RegExp("((^(0x){1,1})([a-fA-F0-9]*)$)|(^([0-9]*)$)");

  // if(regExp.test(val) === false) {
  //   Message("JS RegExpt fault.");
  //   return Invalid;
  // }
  
  if(val === "-") return Intermediate;

  var data = parseInt(val);
  // var data = parseFloat(val);
  // var data = parseDouble(val);

  if(isNaN(data)) {
    Message("JS data is NaN!");
    return Invalid;
  }
  // data *= 0.1;
  Message("JS Validator data = " + data);

  if((data > Attributes.valmax)||(data < Attributes.valmin)) return Invalid;
  return Acceptable;
}

Message("Jave Script Evaluate");

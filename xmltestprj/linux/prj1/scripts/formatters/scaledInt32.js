DebugOut("Start script: scaledInt32");
DebugOut("All attributes:");
var attributes_keys = Object.keys(Attributes);
for(var i = 0; i < attributes_keys.length; i++)
{
  DebugOut("  Attributes[ " + attributes_keys[i] + " ] = " + Attributes[ attributes_keys[i] ]);
}


#include "lcstringdataformatterbase.h"
static const QString __slUndefStateString = "######";
static const QString __slWrongStateString = "??????";

bool LCStringDataFormatterBase::undefState(QString& _string)
{
  _string = __slUndefStateString;
  return true;
}

bool LCStringDataFormatterBase::wrongState(QString& _string)
{
  _string = __slWrongStateString;
  return true;
}

LCStringDataFormatterBase::LCStringDataFormatterBase()
{
}

LCStringDataFormatterBase::~LCStringDataFormatterBase()
{
}


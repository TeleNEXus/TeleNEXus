#include "lcstringdataformatterbase.h"
static const QString __slUndefStateString = "######";
static const QString __slWrongStateString = "??????";

QString LCStringDataFormatterBase::undefStateString()
{
  return __slUndefStateString;
}

QString LCStringDataFormatterBase::wrongStateString()
{
  return __slWrongStateString;
}

LCStringDataFormatterBase::LCStringDataFormatterBase()
{
}

LCStringDataFormatterBase::~LCStringDataFormatterBase()
{
}


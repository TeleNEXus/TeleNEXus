#include "lcformatterbase.h"
static const QString __slUndefStateString = "######";
static const QString __slWrongStateString = "??????";

bool LCFormatterBase::undefState(QString& _string)
{
  _string = __slUndefStateString;
  return true;
}

bool LCFormatterBase::wrongState(QString& _string)
{
  _string = __slWrongStateString;
  return true;
}

LCFormatterBase::LCFormatterBase()
{
}

LCFormatterBase::~LCFormatterBase()
{
}


#include "lcformatterintbase.h"


LCFormatterIntBase::LCFormatterIntBase(  
    int     _fieldWidth,
    QChar   _fillChar,
    int     _base) :  mFieldWidth(_fieldWidth),
  mFillChar(_fillChar),
  mBase(_base)
{};

LCFormatterIntBase::~LCFormatterIntBase()
{
}

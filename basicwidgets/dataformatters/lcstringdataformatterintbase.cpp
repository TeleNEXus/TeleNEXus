#include "lcstringdataformatterintbase.h"


LCStringDataFormatterIntBase::LCStringDataFormatterIntBase(  
    int     _fieldWidth,
    QChar   _fillChar,
    int     _base) :  mFieldWidth(_fieldWidth),
  mFillChar(_fillChar),
  mBase(_base)
{};

LCStringDataFormatterIntBase::~LCStringDataFormatterIntBase()
{
}

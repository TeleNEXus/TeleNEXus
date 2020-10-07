#include "lcstringdataformatterintbase.h"


LCStringDataFormatterIntBase::LCStringDataFormatterIntBase(  int     _fieldWidth,
                                                 QChar   _fillChar,
                                                 int     _base,
                                                 QChar   _fillCharUndef,
                                                 QChar   _fillCharWrong) :  mFieldWidth(_fieldWidth),
                                                                            mFillChar(_fillChar),
                                                                            mBase(_base),
                                                                            mFillCharUndef(_fillCharUndef),
                                                                            mFillCharWrong(_fillCharWrong)
{};

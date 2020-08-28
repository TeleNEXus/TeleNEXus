#include "lcstringdataformatterbase.h"

QChar     LCStringDataFormatterBase::msFillCharUndefDef('#');
quint8    LCStringDataFormatterBase::msFillCharUndefDefLength(4);
QChar     LCStringDataFormatterBase::msFillCharWrongDef('?');
quint8    LCStringDataFormatterBase::msFillCharWrongDefLength(3);

QString LCStringDataFormatterBase::getUndefStateString(int _fieldWidth, QChar _charUndef)
{
    QChar ch = (_charUndef.isNull()) ? (msFillCharUndefDef):(_charUndef);
    if(_fieldWidth == 0) return QString(msFillCharUndefDefLength, ch);
    return QString(abs(_fieldWidth), ch);
}

QString LCStringDataFormatterBase::getWrongStateString(int _fieldWidth, QChar _charWrong)
{
    QChar ch = (_charWrong.isNull()) ? (msFillCharWrongDef):(_charWrong);
    if(_fieldWidth == 0) return QString(msFillCharWrongDefLength, ch);
    return QString(abs(_fieldWidth), ch);
}

void LCStringDataFormatterBase::setFillCharUndefDef(QChar _c, quint8 _length)
{
    msFillCharUndefDef = _c;
    msFillCharUndefDefLength = _length;
}



void LCStringDataFormatterBase::setFillCharWrongDef(QChar _c, quint8 _length)
{
    msFillCharWrongDef = _c;
    msFillCharUndefDefLength = _length;
}



LCStringDataFormatterBase::LCStringDataFormatterBase()
{

}

LCStringDataFormatterBase::~LCStringDataFormatterBase()
{

}


#include "lcdatastrformatbase.h"

namespace remgui
{

QChar     LCDataStrFormatBase::msFillCharUndefDef('#');
TUint8    LCDataStrFormatBase::msFillCharUndefDefLength(4);
QChar     LCDataStrFormatBase::msFillCharWrongDef('?');
TUint8    LCDataStrFormatBase::msFillCharWrongDefLength(3);

QString LCDataStrFormatBase::getUndefStateString(int _fieldWidth, QChar _charUndef)
{
    QChar ch = (_charUndef.isNull()) ? (msFillCharUndefDef):(_charUndef);
    if(_fieldWidth == 0) return QString(msFillCharUndefDefLength, ch);
    return QString(abs(_fieldWidth), ch);
}

QString LCDataStrFormatBase::getWrongStateString(int _fieldWidth, QChar _charWrong)
{
    QChar ch = (_charWrong.isNull()) ? (msFillCharWrongDef):(_charWrong);
    if(_fieldWidth == 0) return QString(msFillCharWrongDefLength, ch);
    return QString(abs(_fieldWidth), ch);
}

void LCDataStrFormatBase::setFillCharUndefDef(QChar _c, TUint8 _length)
{
    msFillCharUndefDef = _c;
    msFillCharUndefDefLength = _length;
}

void LCDataStrFormatBase::setFillCharUndefDef(QChar _c)
{
    msFillCharUndefDef = _c;
}

void LCDataStrFormatBase::setFillCharWrongDef(QChar _c, TUint8 _length)
{
    msFillCharWrongDef = _c;
    msFillCharUndefDefLength = _length;
}

void LCDataStrFormatBase::setFillCharWrongDef(QChar _c)
{
    msFillCharWrongDef = _c;
}

LCDataStrFormatBase::LCDataStrFormatBase()
{

}

}

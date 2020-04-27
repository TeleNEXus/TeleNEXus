#include "lcdatastrformatu16.h"
#include <limits>
#include <math.h>

namespace remgui
{

//================================================================================================LCQDataFormatterUInt16
LCDataStrFormatU16::
    LCDataStrFormatU16(   int     _fieldWidth,
                          QChar   _fillChar,
                          int     _base,
                          QChar   _fillCharUndef,
                          QChar   _fillCharWrong) :
                              LCDataStrFormatIntBase( _fieldWidth,
                                                      _fillChar,
                                                      _base,
                                                      _fillCharUndef,
                                                      _fillCharWrong)
{
    mValidator.setRange(std::numeric_limits<TUint16>::min(), std::numeric_limits<TUint16>::max());
}

//--------------------------------------------------------------------------------------------------------------toString
QString LCDataStrFormatU16::toString(const QByteArray& _data)
{
    if(_data.size() < 2)
    {
        QChar ch = (mFillCharWrong.isNull()) ? (msFillCharWrongDef):(mFillCharWrong);
        int length = (mFieldWidth == 0) ? (msFillCharWrongDefLength) : (abs(mFieldWidth));
        return QString(length, ch);
    }
    return QString("%1").arg( ((TUint16*)_data.constData())[0], mFieldWidth, mBase, mFillChar);
}

//---------------------------------------------------------------------------------------------------------------toBytes
QByteArray LCDataStrFormatU16::toBytes(const QString& _str)
{
    bool ok = false;
    TUint16 r = _str.toUShort(&ok, mBase);
    if(!ok) return QByteArray();
    return QByteArray((char*)(&r), 2);
}

//------------------------------------------------------------------------------------------------------undefStateString
QString LCDataStrFormatU16::undefStateString()
{
    return getUndefStateString(mFieldWidth, mFillCharUndef);
}

//-------------------------------------------------------------------------------------------------------------validator
QValidator* LCDataStrFormatU16::validator()
{
    return &mValidator;
}

}

#include "lcdatastrformats8.h"

#include <limits>
#include <math.h>

namespace remgui
{

//==================================================================================================LCQDataFormatterInt8
LCDataStrFormatS8::
    LCDataStrFormatS8(  int     _fieldWidth,
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
    mValidator.setRange(std::numeric_limits<TInt8>::min(), std::numeric_limits<TInt8>::max());
}

//--------------------------------------------------------------------------------------------------------------toString
QString LCDataStrFormatS8::toString(const QByteArray& _data)
{
    if(_data.size() < 1)
    {
        QChar ch = (mFillCharWrong.isNull()) ? (msFillCharWrongDef):(mFillCharWrong);
        int length = (mFieldWidth == 0) ? (msFillCharWrongDefLength) : (abs(mFieldWidth));
        return QString(length, ch);
    }
    TInt8 r = ((TInt8*)_data.constData())[0];
    return QString("%1").arg( r, mFieldWidth, mBase, mFillChar);
}

//---------------------------------------------------------------------------------------------------------------toBytes
QByteArray LCDataStrFormatS8::toBytes(const QString& _str)
{
    bool ok = false;
    TInt8 r = (TInt8)_str.toShort(&ok, mBase);
    if(!ok) return QByteArray();
    return QByteArray((char*)(&r), 1);
}

//------------------------------------------------------------------------------------------------------undefStateString
QString LCDataStrFormatS8::undefStateString()
{
    return getUndefStateString(mFieldWidth, mFillCharUndef);
}

//-------------------------------------------------------------------------------------------------------------validator
QValidator* LCDataStrFormatS8::validator()
{
    return &mValidator;
}

}



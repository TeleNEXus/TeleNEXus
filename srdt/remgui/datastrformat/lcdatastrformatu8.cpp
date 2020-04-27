#include "lcdatastrformatu8.h"

#include <limits>
#include <math.h>

namespace remgui
{

//=================================================================================================LCQDataFormatterUint8
LCDataStrFormatU8::
    LCDataStrFormatU8(   int     _fieldWidth,
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
    mValidator.setRange(std::numeric_limits<quint8>::min(), std::numeric_limits<quint8>::max());
}

//--------------------------------------------------------------------------------------------------------------toString
QString LCDataStrFormatU8::toString(const QByteArray& _data)
{
    if(_data.size() < 1)
    {
        QChar ch = (mFillCharWrong.isNull()) ? (msFillCharWrongDef):(mFillCharWrong);
        int length = (mFieldWidth == 0) ? (msFillCharWrongDefLength) : (abs(mFieldWidth));
        return QString(length, ch);
    }
    return QString("%1").arg( ((quint8*)_data.constData())[0], mFieldWidth, mBase, mFillChar);
}

//---------------------------------------------------------------------------------------------------------------toBytes
QByteArray LCDataStrFormatU8::toBytes(const QString& _str)
{
    bool ok = false;
    quint16 r = ((quint16)_str.toUShort(&ok, mBase));
    if((!ok) || ((r & 0xff00) != 0)) return QByteArray();
    return QByteArray((char*)(&r), 1);
}

//------------------------------------------------------------------------------------------------------undefStateString
QString LCDataStrFormatU8::undefStateString()
{
    return getUndefStateString(mFieldWidth, mFillCharUndef);
}

//-------------------------------------------------------------------------------------------------------------validator
QValidator* LCDataStrFormatU8::validator()
{
    return &mValidator;
}

}


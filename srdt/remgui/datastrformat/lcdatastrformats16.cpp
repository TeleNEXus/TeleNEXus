#include "lcdatastrformats16.h"

#include <limits>
#include <math.h>

namespace remgui
{

//=================================================================================================LCQDataFormatterInt16
LCDataStrFormatS16::
    LCDataStrFormatS16(   int     _fieldWidth,
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
    mValidator.setRange(std::numeric_limits<qint16>::min(), std::numeric_limits<qint16>::max());
}

//--------------------------------------------------------------------------------------------------------------toString
QString LCDataStrFormatS16::toString(const QByteArray& _data)
{
    if(_data.size() < 2)
    {
        QChar ch = (mFillCharWrong.isNull()) ? (msFillCharWrongDef):(mFillCharWrong);
        int length = (mFieldWidth == 0) ? (msFillCharWrongDefLength) : (abs(mFieldWidth));
        return QString(length, ch);
    }
    return QString("%1").arg( ((qint16*)_data.constData())[0], mFieldWidth, mBase, mFillChar);
}

//---------------------------------------------------------------------------------------------------------------toBytes
QByteArray LCDataStrFormatS16::toBytes(const QString& _str)
{
    bool ok = false;
    qint16 r = _str.toShort(&ok, mBase);
    if(!ok) return QByteArray();
    return QByteArray((char*)(&r), 2);
}

//------------------------------------------------------------------------------------------------------undefStateString
QString LCDataStrFormatS16::undefStateString()
{
    return getUndefStateString(mFieldWidth, mFillCharUndef);
}

//-------------------------------------------------------------------------------------------------------------validator
QValidator* LCDataStrFormatS16::validator()
{
    return &mValidator;
}

}


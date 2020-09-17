#include "lcstringdataformatteru16.h"
#include <limits>
#include <math.h>

//==============================================================================LCQDataFormatterUInt16
LCStringDataFormatterU16::
    LCStringDataFormatterU16(   int     _fieldWidth,
                          QChar   _fillChar,
                          int     _base,
                          QChar   _fillCharUndef,
                          QChar   _fillCharWrong) :
                              LCStringDataFormatterIntBase( _fieldWidth,
                                                      _fillChar,
                                                      _base,
                                                      _fillCharUndef,
                                                      _fillCharWrong)
{
    mValidator.setRange(    std::numeric_limits<quint16>::min(), 
                            std::numeric_limits<quint16>::max());
}

//------------------------------------------------------------------------------toString
QString LCStringDataFormatterU16::toString(const QByteArray& _data)
{
    if(_data.size() < 2)
    {
        QChar ch = (mFillCharWrong.isNull()) ? 
                    (msFillCharWrongDef):(mFillCharWrong);
        int length = (mFieldWidth == 0) ? 
                    (msFillCharWrongDefLength) : (abs(mFieldWidth));
        return QString(length, ch);
    }
    return QString("%1").arg( ((quint16*)_data.constData())[0], 
                                    mFieldWidth, mBase, mFillChar);
}

//------------------------------------------------------------------------------normalizeString
QString LCStringDataFormatterU16::normalizeString(const QString& _str)
{
    bool    ok  = false;
    qint16  val = _str.toUShort(&ok);
    if(!ok) return QString();
    return QString("%1").arg( val, mFieldWidth, mBase, mFillChar);
}

//------------------------------------------------------------------------------toBytes
QByteArray LCStringDataFormatterU16::toBytes(const QString& _str)
{
    bool ok = false;
    quint16 r = _str.toUShort(&ok, mBase);
    if(!ok) return QByteArray();
    return QByteArray((char*)(&r), 2);
}

//------------------------------------------------------------------------------undefStateString
QString LCStringDataFormatterU16::undefStateString()
{
    return getUndefStateString(mFieldWidth, mFillCharUndef);
}

//------------------------------------------------------------------------------validator
QValidator* LCStringDataFormatterU16::validator()
{
    return &mValidator;
}


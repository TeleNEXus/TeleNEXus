#include "lcstringdataformatteru32.h"

//==========================================================================================LCQDataStringFormatterUint32
LCStringDataFormatterU32::
    LCStringDataFormatterU32( int     _fieldWidth,
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
}

//--------------------------------------------------------------------------------------------------------------toString
QString LCStringDataFormatterU32::toString(const QByteArray& _data)
{
    if(_data.size() < 4)
    {
        QChar ch = (mFillCharWrong.isNull()) ? (msFillCharWrongDef):(mFillCharWrong);
        int length = (mFieldWidth == 0) ? (msFillCharWrongDefLength) : (abs(mFieldWidth));
        return QString(length, ch);
    }
    return QString("%1").arg( ((quint32*)_data.constData())[0], mFieldWidth, mBase, mFillChar);
}

//---------------------------------------------------------------------------------------------------------------toBytes
QByteArray LCStringDataFormatterU32::toBytes(const QString& _str)
{
    bool ok = false;
    quint32 r = ((quint32)_str.toUInt(&ok, mBase));
    if(!ok) return QByteArray();
    return QByteArray((char*)(&r), 4);
}

//------------------------------------------------------------------------------------------------------undefStateString
QString LCStringDataFormatterU32::undefStateString()
{
    return getUndefStateString(mFieldWidth, mFillCharUndef);
}

//-------------------------------------------------------------------------------------------------------------validator
QValidator* LCStringDataFormatterU32::validator()
{
    return &mValidator;
}


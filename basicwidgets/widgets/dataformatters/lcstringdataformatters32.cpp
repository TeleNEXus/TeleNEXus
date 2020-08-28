#include "lcstringdataformatters32.h"

//===========================================================================================LCQDataStringFormatterInt32
LCStringDataFormatterS32::
    LCStringDataFormatterS32( int     _fieldWidth,
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
QString LCStringDataFormatterS32::toString(const QByteArray& _data)
{
    if(_data.size() < 4)
    {
        QChar ch = (mFillCharWrong.isNull()) ? (msFillCharWrongDef):(mFillCharWrong);
        int length = (mFieldWidth == 0) ? (msFillCharWrongDefLength) : (abs(mFieldWidth));
        return QString(length, ch);
    }
    return QString("%1").arg( ((qint32*)_data.constData())[0], mFieldWidth, mBase, mFillChar);
}

//---------------------------------------------------------------------------------------------------------------toBytes
QByteArray LCStringDataFormatterS32::toBytes(const QString& _str)
{
    bool ok = false;
    qint32 r = ((qint32)_str.toInt(&ok, mBase));
    if(!ok) return QByteArray();
    return QByteArray((char*)(&r), 4);
}

//------------------------------------------------------------------------------------------------------undefStateString
QString LCStringDataFormatterS32::undefStateString()
{
    return getUndefStateString(mFieldWidth, mFillCharUndef);
}

//-------------------------------------------------------------------------------------------------------------validator
QValidator* LCStringDataFormatterS32::validator()
{
    return &mValidator;
}

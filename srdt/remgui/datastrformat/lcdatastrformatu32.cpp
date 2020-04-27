#include "lcdatastrformatu32.h"

namespace remgui
{

//==========================================================================================LCQDataStringFormatterUint32
LCDataStrFormatU32::
    LCDataStrFormatU32( int     _fieldWidth,
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
}

//--------------------------------------------------------------------------------------------------------------toString
QString LCDataStrFormatU32::toString(const QByteArray& _data)
{
    if(_data.size() < 4)
    {
        QChar ch = (mFillCharWrong.isNull()) ? (msFillCharWrongDef):(mFillCharWrong);
        int length = (mFieldWidth == 0) ? (msFillCharWrongDefLength) : (abs(mFieldWidth));
        return QString(length, ch);
    }
    return QString("%1").arg( ((TUint32*)_data.constData())[0], mFieldWidth, mBase, mFillChar);
}

//---------------------------------------------------------------------------------------------------------------toBytes
QByteArray LCDataStrFormatU32::toBytes(const QString& _str)
{
    bool ok = false;
    TUint32 r = ((TUint32)_str.toUInt(&ok, mBase));
    if(!ok) return QByteArray();
    return QByteArray((char*)(&r), 4);
}

//------------------------------------------------------------------------------------------------------undefStateString
QString LCDataStrFormatU32::undefStateString()
{
    return getUndefStateString(mFieldWidth, mFillCharUndef);
}

//-------------------------------------------------------------------------------------------------------------validator
QValidator* LCDataStrFormatU32::validator()
{
    return &mValidator;
}

}

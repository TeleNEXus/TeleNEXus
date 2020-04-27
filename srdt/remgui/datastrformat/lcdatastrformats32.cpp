#include "lcdatastrformats32.h"

namespace remgui
{

//===========================================================================================LCQDataStringFormatterInt32
LCDataStrFormatS32::
    LCDataStrFormatS32( int     _fieldWidth,
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
QString LCDataStrFormatS32::toString(const QByteArray& _data)
{
    if(_data.size() < 4)
    {
        QChar ch = (mFillCharWrong.isNull()) ? (msFillCharWrongDef):(mFillCharWrong);
        int length = (mFieldWidth == 0) ? (msFillCharWrongDefLength) : (abs(mFieldWidth));
        return QString(length, ch);
    }
    return QString("%1").arg( ((TInt32*)_data.constData())[0], mFieldWidth, mBase, mFillChar);
}

//---------------------------------------------------------------------------------------------------------------toBytes
QByteArray LCDataStrFormatS32::toBytes(const QString& _str)
{
    bool ok = false;
    TInt32 r = ((TInt32)_str.toInt(&ok, mBase));
    if(!ok) return QByteArray();
    return QByteArray((char*)(&r), 4);
}

//------------------------------------------------------------------------------------------------------undefStateString
QString LCDataStrFormatS32::undefStateString()
{
    return getUndefStateString(mFieldWidth, mFillCharUndef);
}

//-------------------------------------------------------------------------------------------------------------validator
QValidator* LCDataStrFormatS32::validator()
{
    return &mValidator;
}

}

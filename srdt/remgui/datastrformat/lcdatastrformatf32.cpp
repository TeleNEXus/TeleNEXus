#include "lcdatastrformatf32.h"

namespace remgui
{

//=========================================================================================LCQDataStringFormatterFloat32
LCDataStrFormatF32::LCDataStrFormatF32(
        int     _fieldWidth,
        char    _format,
        int     _precision,
        int     _decimals,
        QChar   _fillChar,
        QChar   _fillCharUndef,
        QChar   _fillCharWrong) :   mFieldWidth(_fieldWidth),
                                    mFormat(_format),
                                    mPrecision(_precision),
                                    mFillChar(_fillChar),
                                    mFillCharUndef(_fillCharUndef),
                                    mFillCharWrong(_fillCharWrong)
{
    mValidator.setRange((-1.0f)*std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), _decimals);
    mValidator.setLocale(QLocale::c());
}

//--------------------------------------------------------------------------------------------------------------toString
QString LCDataStrFormatF32::toString(const QByteArray& _data)
{
    if(_data.size() < 4)
    {
        QChar ch = (mFillCharWrong.isNull()) ? (msFillCharWrongDef):(mFillCharWrong);
        int length = (mFieldWidth == 0) ? (msFillCharWrongDefLength) : (abs(mFieldWidth));
        return QString(length, ch);
    }
    return QString("%1").arg(((float*)_data.constData())[0], mFieldWidth, mFormat, mPrecision, mFillChar);
}

//---------------------------------------------------------------------------------------------------------------toBytes
QByteArray LCDataStrFormatF32::toBytes(const QString& _str)
{
    bool ok = false;
    float r = ((float)_str.toFloat(&ok));
    if(!ok) return QByteArray();
    return QByteArray((char*)(&r), 4);
}

//------------------------------------------------------------------------------------------------------undefStateString
QString LCDataStrFormatF32::undefStateString()
{
    return getUndefStateString(mFieldWidth, mFillCharUndef);
}

//-------------------------------------------------------------------------------------------------------------validator
QValidator* LCDataStrFormatF32::validator()
{
    return &mValidator;
}

}

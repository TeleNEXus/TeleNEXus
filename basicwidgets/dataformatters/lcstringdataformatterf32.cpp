#include "lcstringdataformatterf32.h"


//==============================================================================LCQDataStringFormatterFloat32
LCStringDataFormatterF32::LCStringDataFormatterF32(
        int     _fieldWidth,
        char    _format,
        int     _precision,
        int     _decimals,
        QChar   _fillChar) :   mFieldWidth(_fieldWidth),
                                    mFormat(_format),
                                    mPrecision(_precision),
                                    mFillChar(_fillChar)
{
    mValidator.setRange(
        (-1.0f)*std::numeric_limits<float>::max(), 
        std::numeric_limits<float>::max(), 
        _decimals);
    mValidator.setLocale(QLocale::c());
}

//------------------------------------------------------------------------------
LCStringDataFormatterF32::~LCStringDataFormatterF32()
{
}

//------------------------------------------------------------------------------toString
QString LCStringDataFormatterF32::toString(const QByteArray& _data)
{
    if(_data.size() < 4)
    {
      return wrongStateString();
    }

    return QString("%1").arg(
            ((float*)_data.constData())[0], 
                    mFieldWidth, mFormat, mPrecision, mFillChar);
}

//------------------------------------------------------------------------------
QString LCStringDataFormatterF32::normalizeString(const QString& _str)
{
    bool ok = false;
    float valf = ((float)_str.toFloat(&ok));

    if(!ok) return QString();

    return QString("%1").arg( valf, 
                                mFieldWidth, mFormat, mPrecision, mFillChar);
}

//------------------------------------------------------------------------------toBytes
QByteArray LCStringDataFormatterF32::toBytes(const QString& _str)
{
    bool ok = false;
    float r = ((float)_str.toFloat(&ok));
    if(!ok) return QByteArray();
    return QByteArray((char*)(&r), 4);
}

//------------------------------------------------------------------------------validator
QValidator* LCStringDataFormatterF32::validator()
{
    return &mValidator;
}


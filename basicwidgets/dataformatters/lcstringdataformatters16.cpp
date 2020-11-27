#include "lcstringdataformatters16.h"

#include <limits>
#include <math.h>
#include <qglobal.h>

//==============================================================================LCQDataFormatterInt16
LCStringDataFormatterS16::LCStringDataFormatterS16(   
    int     _fieldWidth,
    QChar   _fillChar,
    int     _base) :
  LCStringDataFormatterIntBase( _fieldWidth, _fillChar, _base)
{
    mValidator.setRange(std::numeric_limits<qint16>::min(), 
                                         std::numeric_limits<qint16>::max());
}

//------------------------------------------------------------------------------toString
QString LCStringDataFormatterS16::toString(const QByteArray& _data)
{
    if(_data.size() < 2)
    {
      return wrongStateString();
    }
    return QString("%1").arg( 
             ((qint16*)_data.constData())[0], mFieldWidth, mBase, mFillChar);
}

//------------------------------------------------------------------------------normalize
QString LCStringDataFormatterS16::normalize(const QString& _str)
{
    bool    ok = false;
    qint16  val = _str.toShort(&ok);
    if(!ok) return QString();
    return QString("%1").arg( val, mFieldWidth, mBase, mFillChar);
}

//------------------------------------------------------------------------------toBytes
QByteArray LCStringDataFormatterS16::toBytes(const QString& _str)
{
    bool ok = false;
    qint16 r = _str.toShort(&ok, mBase);
    if(!ok) return QByteArray();
    return QByteArray((char*)(&r), 2);
}


//------------------------------------------------------------------------------validator
QValidator* LCStringDataFormatterS16::validator()
{
    return &mValidator;
}



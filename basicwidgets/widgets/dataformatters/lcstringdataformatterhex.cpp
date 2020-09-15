#include "lcstringdataformatterhex.h"
#include <QDebug>
#include <functional>

LCStringDataFormatterHex::LCStringDataFormatterHex(QChar   _separator,
                                       QChar _fillCharUndef,
                                       QChar _fillCharWrong) :  mSeparator(_separator),
                                                                mFillCharUndef(_fillCharUndef),
                                                                mFillCharWrong(_fillCharWrong)
{
}

QString LCStringDataFormatterHex::toString(const QByteArray& _data)
{
    QString str;
    if(_data.size() < 1)
    {
        return QString(msFillCharWrongDefLength, mFillCharWrong);
    }

    for(int i = (_data.size() - 1); i >= 0; i--)
    {
        if(mSeparator.isNull())
        {
            str = str + QString("%1").arg(((quint8*)_data.constData())[i], 2, 16, QChar('0'));
        }
        else
        {
            str = str + QString("%1%2").arg(((quint8*)_data.constData())[i], 2, 16, QChar('0')).arg(mSeparator);
        }
    }

    if(!mSeparator.isNull())
    {
        str.resize(str.length() -1);
    }
    return str;
}


QByteArray LCStringDataFormatterHex::toBytes(const QString& _str)
{
    QByteArray arr;
    int step = (mSeparator.isNull()) ? (2):(3);

    if(_str.length() < 2) return arr;

    for(int i = (_str.length() - 2); i >= 0; i -= step)
    {
        bool conv_flag = false;
        char data = _str.mid(i,2).toShort(&conv_flag, 16);
        if(conv_flag)
        {
            arr.append(1,data);
        }
    }
    return arr;
}

QString     LCStringDataFormatterHex::undefStateString()
{
    return QString(msFillCharUndefDefLength, mFillCharUndef);
}


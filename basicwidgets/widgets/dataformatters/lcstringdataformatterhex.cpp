#include "lcstringdataformatterhex.h"


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
    for(int i = 0; i < _data.size(); i++)
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
    return str;
}


QByteArray LCStringDataFormatterHex::toBytes(const QString& _str)
{
    QByteArray arr;
    bool conv_flag = false;
    int i = 0;
    int step = (mSeparator.isNull()) ? (2):(3);

    if(_str.length() < 2) return arr;
    do
    {
        char data = _str.mid(i,2).toShort(&conv_flag, 16);
        if(conv_flag)
        {
            arr.append(1,data);
        }
        i += step;
    }while(i < _str.length());
    return arr;
}

QString     LCStringDataFormatterHex::undefStateString()
{
    return QString(msFillCharUndefDefLength, mFillCharUndef);
}


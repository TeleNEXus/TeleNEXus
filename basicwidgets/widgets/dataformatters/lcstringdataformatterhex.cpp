#include "lcstringdataformatterhex.h"
#include <QDebug>
#include <functional>

LCStringDataFormatterHex::LCStringDataFormatterHex( 
                                        int   _size,
                                        QChar _separator,
                                        QChar _fillCharUndef,
                                        QChar _fillCharWrong) :  
                                            mSize(_size),
                                            mSeparator(_separator),
                                            mFillCharUndef(_fillCharUndef),
                                            mFillCharWrong(_fillCharWrong)
{
}

//------------------------------------------------------------------------------toString
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
            str = str + QString("%1").arg(
                            ((quint8*)_data.constData())[i], 2, 16, QChar('0'));
        }
        else
        {
            str = str + QString("%1%2").arg(
                            ((quint8*)_data.constData())[i], 2, 16, QChar('0')).
                                arg(mSeparator);
        }
    }

    if(!mSeparator.isNull())
    {
        str.resize(str.length() -1);
    }
    return str;
}

//------------------------------------------------------------------------------normalizeString
QString LCStringDataFormatterHex::normalizeString(const QString& _str)
{
    QString so = _str;

    //Удаляем пробелы.
    so.remove(QRegExp("[ ]{1,}"));

    //Переводим строку в нижний регистр.
    so  = so.toLower(); 
 
    if(mSize < 0)
    {
        //Если размер не задан, то производим нормализацию до четного количества
        //символов и проверку значений.
        if( (_str.size() % 2 ) != 0)
        {
            //Добавляем незначащий ноль.
            so.insert(0, '0');
        }
        if(so.contains(QRegExp("[^0-9,a-f]{1,}")) != 0)
        {
            //Если присутствуют посторонние символы, 
            //возвращаем пустую строку.
            return QString();
        }
        return so;
    }

    so.remove(QRegExp("^[0]{1,}"));
    if( (_str.size() % 2 ) != 0)
    {
        //Добавляем незначащий ноль.
        so.insert(0, '0');
    }

    if((mSize * 2) < so.size())
    {
        return QString();
    }

    if ((mSize * 2) == so.size()) return so;

    so.insert(0, QString( (mSize * 2) - so.size(), '0'));

    return so;
}

//------------------------------------------------------------------------------toBytes
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

//------------------------------------------------------------------------------undefStateString
QString     LCStringDataFormatterHex::undefStateString()
{
    return QString(msFillCharUndefDefLength, mFillCharUndef);
}


#include "lcstringdataformatterbits.h"
#include <QDebug>
#include <functional>

LCStringDataFormatterBits::LCStringDataFormatterBits( 
                                        int   _size,
                                        QChar _fillCharUndef,
                                        QChar _fillCharWrong) :  
                                            mSize(_size),
                                            mFillCharUndef(_fillCharUndef),
                                            mFillCharWrong(_fillCharWrong)
{
}

//------------------------------------------------------------------------------toString
QString LCStringDataFormatterBits::toString(const QByteArray& _data)
{
    QString str;
    if(_data.size() < 1)
    {
        return QString(msFillCharWrongDefLength, mFillCharWrong);
    }

    for(int i = (_data.size() - 1); i >= 0; i--)
    {
        if((quint8)_data.constData()[i] != 0)
        {
            str += '1';
        }
        else
        {
            str += '0';
        }
    }
    return str;
}

//------------------------------------------------------------------------------normalizeString
QString LCStringDataFormatterBits::normalizeString(const QString& _str)
{
    QString so = _str;

    //Удаляем пробелы.
    so.remove(QRegExp("[ ]{1,}"));
    // Проверяем задан ли размер битового поля. 
    if(mSize < 0)
    {
        //Если размер не задан.
        //проверяем на наличие не цифровых значений.
        if(so.contains(QRegExp("[^0-1]{1,}")) != 0)
        {
            //Если присутствуют не цифровые значения, 
            //возвращаем пустую строку.
            return QString();
        }
        return so;
    }
    //Удаление незначащих нулуй в начале последовательности.
    so.remove(QRegExp("^[0]{1,}"));

    if( mSize < so.size())
    {
        return QString();
    }

    if ( mSize == so.size()) return so;

    //Добавление нужное количество незначащих нулуй последовательности. 
    so.insert(0, QString( mSize - so.size(), '0'));

    return so;
}

//------------------------------------------------------------------------------toBytes
QByteArray LCStringDataFormatterBits::toBytes(const QString& _str)
{
    QByteArray arr;
    for(int i = (_str.length() - 1); i >= 0; i -= 1)
    {
        if(_str[i] == '0')
        {
            arr.append(1, 0);
        }
        else
        {
            arr.append(1, 1);
        }
    }
    return arr;
}

//------------------------------------------------------------------------------undefStateString
QString     LCStringDataFormatterBits::undefStateString()
{
    return QString(msFillCharUndefDefLength, mFillCharUndef);
}


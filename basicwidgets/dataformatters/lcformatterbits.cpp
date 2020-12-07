#include "lcformatterbits.h"
#include <QDebug>
#include <functional>

//==============================================================================
LCFormatterBits::
    CValidator::CValidator(int _size, QChar _separator, QObject *_parent) : 
    QValidator(_parent),
    mSize(_size),
    mSeparator(_separator)
{
}

//-----------------------------------------------------------------------------
QValidator::State 
LCFormatterBits::
CValidator::validate(QString &_input, int& _pos) const
{
    Q_UNUSED(_pos);
    QString instr = _input;
    instr.remove(QRegExp(QString("[ _%1]").arg(mSeparator)));
    if(instr.isNull())
    {
        return State::Intermediate;
    }
    //Определить наличие нечисловых значений после удаления 
    //сепараторов.
    if( instr.contains( QRegExp( QString("[^0-1]{1,}"))) )
    {
        return State::Invalid;
    }
    //Проверка длины строки при явном указании длины
    //данных в байтах.
    if( ( mSize > 0) && (instr.size() > mSize ))
    {
        return State::Invalid;
    }

    return State::Acceptable;
}

//==============================================================================
LCFormatterBits::LCFormatterBits( 
        int     _size,
        QChar   _separator) :  
    mValidator(_size, _separator)
{
}

//------------------------------------------------------------------------------
LCFormatterBits::
LCFormatterBits( const LCFormatterBits& _formatter)
{
    mValidator.mSize      = _formatter.mValidator.mSize;         
    mValidator.mSeparator = _formatter.mValidator.mSeparator;    
}
//------------------------------------------------------------------------------
LCFormatterBits::~LCFormatterBits()
{
}

LCFormatterBits& 
LCFormatterBits::operator=(const LCFormatterBits& _formatter)
{
    mValidator.mSize          = _formatter.mValidator.mSize;         
    mValidator.mSeparator     = _formatter.mValidator.mSeparator;    
    return *this;
}
//------------------------------------------------------------------------------toString
QString LCFormatterBits::toString(const QByteArray& _data)
{
    QString out_str;

    if(_data.size() < 1)
    {
      out_str = "Wrong";
      wrongState(out_str);
      return out_str;
    }

    for(int i = (_data.size() - 1); i >= 0; i--)
    {
        if((quint8)_data.constData()[i] != 0)
        {
            out_str += '1';
        }
        else
        {
            out_str += '0';
        }

        if(!mValidator.mSeparator.isNull())
        {
            if( (i - 1) >= 0) out_str += ' ';
        }
    }
    return out_str;
}

//------------------------------------------------------------------------------normalize
QString LCFormatterBits::normalize(const QString& _str)
{
    QString out_str = _str;

    //Удаляем разделительные символы.
    out_str.remove(QRegExp(QString("[ _%1]{1,}").arg(mValidator.mSeparator)));

    // Проверяем задан ли размер битового поля. 
    if(mValidator.mSize <= 0)
    {
        //Если размер не задан.
        //проверяем на наличие не цифровых значений.
        if(out_str.contains(QRegExp("[^0-1]{1,}")) != 0)
        {
            //Если присутствуют не цифровые значения, 
            //возвращаем пустую строку.
            return QString();
        }
        return out_str;
    }

    //Удаление незначащих нулуй в начале последовательности.
    out_str.remove(QRegExp("^[0]{1,}"));


    if( mValidator.mSize < out_str.size())
    {
        out_str.remove(0, out_str.size() - mValidator.mSize);
    }
    else if(mValidator.mSize > out_str.size())
    {
        out_str.insert(0, QString( mValidator.mSize - out_str.size(), '0'));
    }

    /* if( mSize < out_str.size()) */
    /* { */
    /*     return QString(); */
    /* } */

    return out_str;
}

//------------------------------------------------------------------------------toBytes
QByteArray LCFormatterBits::toBytes(const QString& _str)
{
    QByteArray arr;
    QString instr = _str;

    instr.remove(QRegExp(QString("[ _%1]{1,}").arg(mValidator.mSeparator)));

    for(int i = (instr.length() - 1); i >= 0; i -= 1)
    {
        if(instr[i] == '0')
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

//------------------------------------------------------------------------------setSize
void LCFormatterBits::setSize(int _size)
{
    mValidator.mSize = _size;
}

//------------------------------------------------------------------------------setSeparator
void LCFormatterBits::setSeparator(QChar _separator)
{
    mValidator.mSeparator = _separator;
}

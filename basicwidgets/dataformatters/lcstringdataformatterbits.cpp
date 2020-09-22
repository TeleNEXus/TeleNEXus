#include "lcstringdataformatterbits.h"
#include <QDebug>
#include <functional>

//==============================================================================
LCStringDataFormatterBits::
    CValidator::CValidator(const int& _size, const QChar& _separator, QObject *_parent) : 
    QValidator(_parent),
    mSize(_size),
    mSeparator(_separator)
{
}

//-----------------------------------------------------------------------------
QValidator::State 
LCStringDataFormatterBits::
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
LCStringDataFormatterBits::LCStringDataFormatterBits( 
        int     _size,
        QChar   _separator,
        QChar   _fillCharUndef,
        QChar   _fillCharWrong) :  
    mSize(_size),
    mSeparator(_separator),
    mFillCharUndef(_fillCharUndef),
    mFillCharWrong(_fillCharWrong)
{
    mpValidator = new CValidator(mSize, mSeparator);
}

//------------------------------------------------------------------------------
LCStringDataFormatterBits::
LCStringDataFormatterBits( const LCStringDataFormatterBits& _formatter)
{
    this->mSize          = _formatter.mSize;         
    this->mSeparator     = _formatter.mSeparator;    
    this->mFillCharUndef = _formatter.mFillCharUndef;
    this->mFillCharWrong = _formatter.mFillCharWrong;
}
//------------------------------------------------------------------------------
LCStringDataFormatterBits::~LCStringDataFormatterBits()
{
    mpValidator->deleteLater();
}

LCStringDataFormatterBits& 
LCStringDataFormatterBits::operator=(const LCStringDataFormatterBits& _formatter)
{
    this->mSize          = _formatter.mSize;         
    this->mSeparator     = _formatter.mSeparator;    
    this->mFillCharUndef = _formatter.mFillCharUndef;
    this->mFillCharWrong = _formatter.mFillCharWrong;
    return *this;
}
//------------------------------------------------------------------------------toString
QString LCStringDataFormatterBits::toString(const QByteArray& _data)
{
    QString out_str;

    if(_data.size() < 1)
    {
        return QString(msFillCharWrongDefLength, mFillCharWrong);
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

        if(!mSeparator.isNull())
        {
            if( (i - 1) >= 0) out_str += ' ';
        }
    }
    return out_str;
}

//------------------------------------------------------------------------------normalizeString
QString LCStringDataFormatterBits::normalizeString(const QString& _str)
{
    QString out_str = _str;

    //Удаляем разделительные символы.
    out_str.remove(QRegExp(QString("[ _%1]{1,}").arg(mSeparator)));

    // Проверяем задан ли размер битового поля. 
    if(mSize < 0)
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


    if( mSize < out_str.size())
    {
        out_str.remove(0, out_str.size() - mSize);
    }
    else if(mSize > out_str.size())
    {
        out_str.insert(0, QString( mSize - out_str.size(), '0'));
    }

    /* if( mSize < out_str.size()) */
    /* { */
    /*     return QString(); */
    /* } */

    return out_str;
}

//------------------------------------------------------------------------------toBytes
QByteArray LCStringDataFormatterBits::toBytes(const QString& _str)
{
    QByteArray arr;
    QString instr = _str;

    instr.remove(QRegExp(QString("[ _%1]{1,}").arg(mSeparator)));

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

//------------------------------------------------------------------------------undefStateString
QString     LCStringDataFormatterBits::undefStateString()
{
    return QString(msFillCharUndefDefLength, mFillCharUndef);
}

//------------------------------------------------------------------------------setSize
void LCStringDataFormatterBits::setSize(int _size)
{
    mSize = _size;
}

//------------------------------------------------------------------------------setSeparator
void LCStringDataFormatterBits::setSeparator(QChar _separator)
{
    this->mSeparator = _separator;
}

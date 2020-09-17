#include "lcstringdataformatterbool.h"

LCStringDataFormatterBool::LCStringDataFormatterBool(const QString& _strFalse,
                                         const QString& _strTrue,
                                         QChar   _fillCharUndef,
                                         QChar   _fillCharWrong) :  mStrFalse(_strFalse),
                                                                    mStrTrue(_strTrue),
                                                                    mFillCharUndef(_fillCharUndef),
                                                                    mFillCharWrong(_fillCharWrong)
{

}

//------------------------------------------------------------------------------
QString LCStringDataFormatterBool::toString(const QByteArray& _data)
{
    if(_data.size() == 0)
    {
        return getWrongStateString(   (mStrTrue.size() > mStrFalse.size()) ?
                                            (mStrTrue.size()) : (mStrFalse.size()),
                                        mFillCharWrong);
    }

    quint8 byte = 0;
    for(int i = 0; i < _data.size(); i++)
    {
        byte |= _data[i];
    }
    if(byte != 0) return mStrTrue;
    return mStrFalse;
}

//------------------------------------------------------------------------------normalizeString
QString LCStringDataFormatterBool::normalizeString(const QString& _str)
{
    //Удаление незначащих нулей.
    //Удаление сепараторов.
    return _str;
}

//------------------------------------------------------------------------------
QByteArray LCStringDataFormatterBool::toBytes(const QString& _str)
{
    if(_str == mStrTrue) return QByteArray(1,1);
    else if(_str == mStrFalse) return QByteArray(1,0);
    return QByteArray();
}

//------------------------------------------------------------------------------
QString     LCStringDataFormatterBool::undefStateString()
{
    return getUndefStateString((mStrTrue.size() > mStrFalse.size()) ?
                                     (mStrTrue.size()) : (mStrFalse.size()), mFillCharUndef);
}


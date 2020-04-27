#include "lcdatastrformatbool.h"

namespace remgui
{

LCDataStrFormatBool::LCDataStrFormatBool(const QString& _strFalse,
                                         const QString& _strTrue,
                                         QChar   _fillCharUndef,
                                         QChar   _fillCharWrong) :  mStrFalse(_strFalse),
                                                                    mStrTrue(_strTrue),
                                                                    mFillCharUndef(_fillCharUndef),
                                                                    mFillCharWrong(_fillCharWrong)
{

}

QString LCDataStrFormatBool::toString(const QByteArray& _data)
{
    if(_data.size() == 0)
    {
        return getWrongStateString(   (mStrTrue.size() > mStrFalse.size()) ?
                                            (mStrTrue.size()) : (mStrFalse.size()),
                                        mFillCharWrong);
    }

    TUint8 byte = 0;
    for(int i = 0; i < _data.size(); i++)
    {
        byte |= _data[i];
    }
    if(byte != 0) return mStrTrue;
    return mStrFalse;
}

QByteArray LCDataStrFormatBool::toBytes(const QString& _str)
{
    if(_str == mStrTrue) return QByteArray(1,1);
    else if(_str == mStrFalse) return QByteArray(1,0);
    return QByteArray();
}

QString     LCDataStrFormatBool::undefStateString()
{
    return getUndefStateString((mStrTrue.size() > mStrFalse.size()) ?
                                     (mStrTrue.size()) : (mStrFalse.size()), mFillCharUndef);
}

}//namespace

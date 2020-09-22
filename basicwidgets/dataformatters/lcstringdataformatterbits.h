#ifndef LCSTRINGDATAFORMATTERBITS_H
#define LCSTRINGDATAFORMATTERBITS_H

#include "lcstringdataformatterbase.h"

class LCStringDataFormatterBits : public LCStringDataFormatterBase
{
private:
    int     mSize;              //Заданный размер последовательности в битах.
    QChar   mFillCharUndef;     //Заполнение при отсутствии значения.
    QChar   mFillCharWrong;     //Заполнение при ошибочном значении.
public:
    explicit LCStringDataFormatterBits(    
            int     _size           = -1, 
            QChar   _fillCharUndef  = msFillCharUndefDef,
            QChar   _fillCharWrong  = msFillCharWrongDef);
    virtual ~LCStringDataFormatterBits(){}
    virtual QString     toString(const QByteArray& _data) override;
    virtual QString     normalizeString(const QString& _str) override;
    virtual QByteArray  toBytes(const QString& _str) override;
    virtual QString     undefStateString() override;
    virtual QValidator* validator() override {return nullptr;}

    void setFillCharUndef(QChar _c){mFillCharUndef = _c;}
    void setFillCharWrong(QChar _c){mFillCharWrong = _c;}
    void setSize(int _size){mSize = _size;}
};

#endif // LCSTRINGDATAFORMATTERBITS_H



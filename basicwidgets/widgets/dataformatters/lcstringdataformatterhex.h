#ifndef LCSTRINGDATAFORMATTERHEX_H
#define LCSTRINGDATAFORMATTERHEX_H

#include "lcstringdataformatterbase.h"

class LCStringDataFormatterHex : public LCStringDataFormatterBase
{
private:
    QChar   mSeparator;         //Разделитель между отображаемыми числами.
    QChar   mFillCharUndef;     //Заполнение при отсутствии значения.
    QChar   mFillCharWrong;     //Заполнение при ошибочном значении.
public:
    explicit LCStringDataFormatterHex(    QChar   _separator      = QChar(),
                                    QChar   _fillCharUndef  = msFillCharUndefDef,
                                    QChar   _fillCharWrong  = msFillCharWrongDef);
    virtual ~LCStringDataFormatterHex(){}
    virtual QString     toString(const QByteArray& _data) override;
    virtual QByteArray  toBytes(const QString& _str) override;
    virtual QString     undefStateString() override;
    virtual QValidator* validator() override {return nullptr;}

    void setFillCharUndef(QChar _c){mFillCharUndef = _c;}
    void setFillCharWrong(QChar _c){mFillCharWrong = _c;}
};


#endif // LCSTRINGDATAFORMATTERHEX_H

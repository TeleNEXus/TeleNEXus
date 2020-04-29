#ifndef LCDATAFORMATBOOL_H
#define LCDATAFORMATBOOL_H

#include "lcstringdataformatterbase.h"

class LCStringDataFormatterBool : public LCStringDataFormatterBase
{
private:
    QString mStrFalse;
    QString mStrTrue;
    QChar   mFillCharUndef;     //Заполнение при отсутствии значения.
    QChar   mFillCharWrong;     //Заполнение при ошибочном значении.
public:
    explicit LCStringDataFormatterBool(   const QString& _strFalse = "false",
                                    const QString& _strTrue = "true",
                                    QChar   _fillCharUndef  = QChar(),
                                    QChar   _fillCharWrong  = QChar());
    virtual ~LCStringDataFormatterBool(){}
    virtual QString     toString(const QByteArray& _data) override;
    virtual QByteArray  toBytes(const QString& _str) override;
    virtual QString     undefStateString() override;
    virtual QValidator* validator() override {return nullptr;}

    void setFalseString(const QString _str){mStrFalse = _str;}
    void setTrueString(const QString _str){mStrTrue = _str;}
    void setFillCharUndef(QChar _c){mFillCharUndef = _c;}
    void setFillCharWrong(QChar _c){mFillCharWrong = _c;}
};


#endif // LCDATAFORMATBOOL_H

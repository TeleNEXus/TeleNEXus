#ifndef LCDATAFORMATBOOL_H
#define LCDATAFORMATBOOL_H

#include "lcdatastrformatbase.h"

namespace remgui
{

class LCDataStrFormatBool : public LCDataStrFormatBase
{
private:
    QString mStrFalse;
    QString mStrTrue;
    QChar   mFillCharUndef;     //Заполнение при отсутствии значения.
    QChar   mFillCharWrong;     //Заполнение при ошибочном значении.
public:
    explicit LCDataStrFormatBool(   const QString& _strFalse = "false",
                                    const QString& _strTrue = "true",
                                    QChar   _fillCharUndef  = QChar(),
                                    QChar   _fillCharWrong  = QChar());
    virtual ~LCDataStrFormatBool(){}
    virtual QString     toString(const QByteArray& _data) override;
    virtual QByteArray  toBytes(const QString& _str) override;
    virtual QString     undefStateString() override;
    virtual QValidator* validator() override {return nullptr;}

    void setFalseString(const QString _str){mStrFalse = _str;}
    void setTrueString(const QString _str){mStrTrue = _str;}
    void setFillCharUndef(QChar _c){mFillCharUndef = _c;}
    void setFillCharWrong(QChar _c){mFillCharWrong = _c;}
};

}//namespace

#endif // LCDATAFORMATBOOL_H

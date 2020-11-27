#ifndef LCSTRINGDATAFORMATTERFLOAT32_H
#define LCSTRINGDATAFORMATTERFLOAT32_H

#include "lcformatterbase.h"
#include <QDoubleValidator>


class LCFormatterF32 : public LCFormatterBase
{
private:
    int     mFieldWidth;        //Ширина поля.
    char    mFormat;
    int     mPrecision;
    QChar   mFillChar;          //Символ заполнения выравнивания.

    QDoubleValidator mValidator;   //Контроллер диапазона ввода.

public:
    explicit LCFormatterF32(    int     _fieldWidth     = 0,
                                          char    _format         = 'g',
                                          int     _precision      = -1,
                                          int     _decimals       = 1000,
                                          QChar   _fillChar       = QChar(' '));
    virtual ~LCFormatterF32();
    virtual QString     toString(const QByteArray& _data) override;
    virtual QString     normalize(const QString& _str) override; 
    virtual QByteArray  toBytes(const QString& _str) override;
    virtual QValidator* validator() override;

    void setFieldWidth(int _fieldWidth){mFieldWidth = _fieldWidth;}
    void setFillChar(QChar _c){mFillChar = _c;}
    void setFormat(char _format){mFormat = _format;}
    void setPrecision(int _precision){mPrecision = _precision;}
};


#endif // LCSTRINGDATAFORMATTERFLOAT32_H

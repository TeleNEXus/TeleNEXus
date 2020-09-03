﻿#ifndef LCSTRINGDATAFORMATTERFLOAT32_H
#define LCSTRINGDATAFORMATTERFLOAT32_H

#include "lcstringdataformatterbase.h"
#include <QDoubleValidator>


class LCStringDataFormatterF32 : public LCStringDataFormatterBase
{
private:
    int     mFieldWidth;        //Ширина поля.
    char    mFormat;
    int     mPrecision;
    QChar   mFillChar;          //Символ заполнения выравнивания.
    QChar   mFillCharUndef;     //Заполнение при отсутствии значения.
    QChar   mFillCharWrong;     //Заполнение при ошибочном значении.

    QDoubleValidator mValidator;   //Контроллер диапазона ввода.

public:
    explicit LCStringDataFormatterF32( int     _fieldWidth     = 0,
                                            char    _format         = 'g',
                                            int     _precision      = -1,
                                            int     _decimals       = 1000,
                                            QChar   _fillChar       = QChar(' '),
                                            QChar   _fillCharUndef  = QChar(),
                                            QChar   _fillCharWrong  = QChar());

    virtual QString     toString(const QByteArray& _data) override;
    virtual QByteArray  toBytes(const QString& _str) override;
    virtual QString     undefStateString() override;
    virtual QValidator* validator() override;

    void setFieldWidth(int _fieldWidth){mFieldWidth = _fieldWidth;}
    void setFillChar(QChar _c){mFillChar = _c;}
    void setFormat(char _format){mFormat = _format;}
    void setPrecision(int _precision){mPrecision = _precision;}
    void setFillCharUndef(QChar _c){mFillCharUndef = _c;}
    void setFillCharWrong(QChar _c){mFillCharWrong = _c;}
};


#endif // LCSTRINGDATAFORMATTERFLOAT32_H
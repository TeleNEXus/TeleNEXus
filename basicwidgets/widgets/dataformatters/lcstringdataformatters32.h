﻿#ifndef LCSTRINGDATAFORMATTERINT32_H
#define LCSTRINGDATAFORMATTERINT32_H

#include "lcstringdataformatterintbase.h"
#include <QIntValidator>

class LCStringDataFormatterS32 : public LCStringDataFormatterIntBase
{
private:

    QIntValidator mValidator;   //Контроллер диапазона ввода.

public:
    explicit LCStringDataFormatterS32( int     _fieldWidth     = 0,
                                          QChar   _fillChar       = QChar(' '),
                                          int     _base           = 10,
                                          QChar   _fillCharUndef  = QChar(),
                                          QChar   _fillCharWrong  = QChar());

    virtual QString     toString(const QByteArray& _data) override;
    virtual QString     normalizeString(const QString& _str) override;
    virtual QByteArray  toBytes(const QString& _str) override;
    virtual QString     undefStateString() override;
    virtual QValidator* validator() override;
};


#endif // LCSTRINGDATAFORMATTERINT32_H

#ifndef LCSTRINGDATAFORMATTERINT8_H
#define LCSTRINGDATAFORMATTERINT8_H

#include "lcstringdataformatterintbase.h"

class LCStringDataFormatterS8 : public LCStringDataFormatterIntBase
{
private:

    QIntValidator mValidator;   //Контроллер диапазона ввода.

public:
    explicit LCStringDataFormatterS8(   int     _fieldWidth     = 0,
                                        QChar   _fillChar       = QChar(' '),
                                        int     _base           = 10);

    virtual QString     toString(const QByteArray& _data) override;
    virtual QString     normalize(const QString& _str) override;
    virtual QByteArray  toBytes(const QString& _str) override;
    virtual QValidator* validator() override;
};


#endif // LCSTRINGDATAFORMATTERINT8_H

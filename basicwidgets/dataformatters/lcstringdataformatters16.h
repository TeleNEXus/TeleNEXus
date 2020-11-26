#ifndef LCSTRINGDATAFORMATTERINT16_H
#define LCSTRINGDATAFORMATTERINT16_H

#include "lcstringdataformatterintbase.h"
#include <QIntValidator>

class LCStringDataFormatterS16 : public LCStringDataFormatterIntBase
{
private:

    QIntValidator mValidator;   //Контроллер диапазона ввода.

public:
    explicit LCStringDataFormatterS16(    
        int     _fieldWidth     = 0,
        QChar   _fillChar       = QChar(' '),
        int     _base           = 10);


    virtual QString     toString(const QByteArray& _data) override;
    QString normalizeString(const QString& _str) override;
    virtual QByteArray  toBytes(const QString& _str) override;
    virtual QValidator* validator() override;
};


#endif // LCSTRINGDATAFORMATTERINT16_H

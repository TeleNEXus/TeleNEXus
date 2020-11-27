#ifndef LCSTRINGDATAFORMATTERINT32_H
#define LCSTRINGDATAFORMATTERINT32_H

#include "lcformatterintbase.h"
#include <QIntValidator>

class LCFormatterS32 : public LCFormatterIntBase
{
private:

    QIntValidator mValidator;   //Контроллер диапазона ввода.

public:
    explicit LCFormatterS32( 
        int     _fieldWidth     = 0,
        QChar   _fillChar       = QChar(' '),
        int     _base           = 10);

    virtual QString     toString(const QByteArray& _data) override;
    virtual QString     normalize(const QString& _str) override;
    virtual QByteArray  toBytes(const QString& _str) override;
    virtual QValidator* validator() override;
};


#endif // LCSTRINGDATAFORMATTERINT32_H

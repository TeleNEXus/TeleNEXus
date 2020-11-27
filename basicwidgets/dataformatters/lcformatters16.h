#ifndef LCSTRINGDATAFORMATTERINT16_H
#define LCSTRINGDATAFORMATTERINT16_H

#include "lcformatterintbase.h"
#include <QIntValidator>

class LCFormatterS16 : public LCFormatterIntBase
{
private:

    QIntValidator mValidator;   //Контроллер диапазона ввода.

public:
    explicit LCFormatterS16(    
        int     _fieldWidth     = 0,
        QChar   _fillChar       = QChar(' '),
        int     _base           = 10);


    virtual QString     toString(const QByteArray& _data) override;
    QString normalize(const QString& _str) override;
    virtual QByteArray  toBytes(const QString& _str) override;
    virtual QValidator* validator() override;
};


#endif // LCSTRINGDATAFORMATTERINT16_H

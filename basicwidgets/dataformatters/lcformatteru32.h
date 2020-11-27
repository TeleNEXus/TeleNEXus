#ifndef LCSTRINGDATAFORMATTERUINT32_H
#define LCSTRINGDATAFORMATTERUINT32_H

#include "lcformatterintbase.h"
#include "lqvalidatoru32.h"


class LCFormatterU32 : public LCFormatterIntBase
{
private:

    LQValidatorU32 mValidator;   //Контроллер диапазона ввода.

public:
    explicit LCFormatterU32( int     _fieldWidth     = 0,
                                          QChar   _fillChar       = QChar(' '),
                                          int     _base           = 10);

    virtual QString     toString(const QByteArray& _data) override;
    virtual QString     normalize(const QString& _str) override;
    virtual QByteArray  toBytes(const QString& _str) override;
    virtual QValidator* validator() override;
};


#endif // LCSTRINGDATAFORMATTERUINT32_H

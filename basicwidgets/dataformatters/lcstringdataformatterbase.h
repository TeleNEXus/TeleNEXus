#ifndef LCSTRINGDATAFORMATTERBASE_H
#define LCSTRINGDATAFORMATTERBASE_H

#include <QString>
#include <QByteArray>
#include <QValidator>

#include "LIStringDataFormatter.h"


class LCStringDataFormatterBase : public LIStringDataFormatter
{
public:
    LCStringDataFormatterBase();
    virtual ~LCStringDataFormatterBase();

    virtual bool undefState(QString& _string) override;
    virtual bool wrongState(QString& _string) override;
};


#endif //LCSTRINGDATAFORMATTERBASE_H

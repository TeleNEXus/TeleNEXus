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

    virtual QString wrongStateString() override;
    virtual QString undefStateString() override;
};


#endif //LCSTRINGDATAFORMATTERBASE_H

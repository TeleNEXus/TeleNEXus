#ifndef LCSTRINGDATAFORMATTERBASE_H
#define LCSTRINGDATAFORMATTERBASE_H

#include <QString>
/* #include <QValidator> */
#include "LIDataFormatter.h"

class LCFormatterBase : public LIDataFormatter
{
public:
    LCFormatterBase();
    virtual ~LCFormatterBase();

    virtual bool undefState(QString& _string) override;
    virtual bool wrongState(QString& _string) override;
};


#endif //LCSTRINGDATAFORMATTERBASE_H

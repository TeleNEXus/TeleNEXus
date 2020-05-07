#ifndef LCXMLSTDDATAFORMATTERFACTORY_H
#define LCXMLSTDDATAFORMATTERFACTORY_H

#include "lcstringdataformatterbase.h"
#include <QDomElement>

using LCStringDataFormatter = LCStringDataFormatterBase;

class LCXmlStdDataFormatterFactory final
{
private:
    LCXmlStdDataFormatterFactory();
    LCXmlStdDataFormatterFactory(const LCXmlStdDataFormatterFactory&) = delete;
    LCXmlStdDataFormatterFactory& operator=(const LCXmlStdDataFormatterFactory&) = delete;
public:
    static LCXmlStdDataFormatterFactory& instance();
    QSharedPointer<LCStringDataFormatter> createStringFormatter(const QDomNamedNodeMap& _attr);
};

#endif // LCXMLSTDDATAFORMATTERFACTORY_H

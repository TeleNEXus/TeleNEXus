#ifndef LCXMLSTDDATAFORMATTERFACTORY_H
#define LCXMLSTDDATAFORMATTERFACTORY_H

#include "LIDataFormatter.h"
#include <QSharedPointer>
#include <QDomElement>


class LCXmlStdDataFormatterFactory final
{
private:
    LCXmlStdDataFormatterFactory();
    LCXmlStdDataFormatterFactory(const LCXmlStdDataFormatterFactory&) = delete;
    LCXmlStdDataFormatterFactory& 
      operator=(const LCXmlStdDataFormatterFactory&) = delete;
public:
    static LCXmlStdDataFormatterFactory& instance();
    QSharedPointer<LIDataFormatter> 
      createStringFormatter(const QDomNamedNodeMap& _attr);
};

#endif // LCXMLSTDDATAFORMATTERFACTORY_H

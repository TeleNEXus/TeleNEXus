#ifndef LCXMLSTDDATAFORMATTERFACTORY_H
#define LCXMLSTDDATAFORMATTERFACTORY_H

#include "lcdatastrformatbase.h"
#include <QDomElement>

using LCStringDataFormatter = remgui::LCDataStrFormatBase;

class LCXmlStdDataFormatterFactory final
{
private:
    LCXmlStdDataFormatterFactory();
    LCXmlStdDataFormatterFactory(const LCXmlStdDataFormatterFactory&) = delete;
    LCXmlStdDataFormatterFactory& operator=(const LCXmlStdDataFormatterFactory&) = delete;
public:
    static LCXmlStdDataFormatterFactory& instance();
    QSharedPointer<LCStringDataFormatter> create(const QDomNamedNodeMap& _attr);
};

#endif // LCXMLSTDDATAFORMATTERFACTORY_H

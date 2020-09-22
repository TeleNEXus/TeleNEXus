#ifndef LCXMLSTDDATAFORMATTERFACTORYINTERFACE_H
#define LCXMLSTDDATAFORMATTERFACTORYINTERFACE_H


#include <QDomNamedNodeMap>

class LCXmlStringDataFormatterInterface;

class LCXmlStdDataFormatterFactoryInterface
{
public:
    LCXmlStdDataFormatterFactoryInterface(){}
    virtual LCXmlStringDataFormatterInterface* createStringFormatter(const QDomNamedNodeMap& _attr) = 0;
};

#endif // LCXMLSTDDATAFORMATTERFACTORYINTERFACE_H

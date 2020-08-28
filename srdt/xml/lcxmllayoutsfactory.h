#ifndef LCLAYOUTSFACTORY_H
#define LCLAYOUTSFACTORY_H

#include <QDomElement>
#include <QLayout>

class LCXmlLayoutsFactory
{
private:
    static LCXmlLayoutsFactory instance;
    LCXmlLayoutsFactory();
    LCXmlLayoutsFactory(const LCXmlLayoutsFactory&) = delete;
    LCXmlLayoutsFactory& operator=(const LCXmlLayoutsFactory&) = delete;
public:
    static QLayout* create(const QDomElement &_element);
};

#endif // LCLAYOUTSFACTORY_H

#ifndef LIXMLWIDGETBUILDER_H
#define LIXMLWIDGETBUILDER_H

class QWidget;
class QDomElement;
class LIApplication;
class LIWindow;

class LIXmlWidgetBuilder
{
public:
    LIXmlWidgetBuilder(){}
    virtual ~LIXmlWidgetBuilder(){}
    virtual QWidget* build( 
            const QDomElement&      _element, 
            const LIApplication&    _app) = 0;
};

#endif // LIXMLWIDGETBUILDER_H

#ifndef LIXMLLAYOUTBUILDER_H
#define LIXMLLAYOUTBUILDER_H

class QLayout;
class QDomElement;
class LIApplication;
class LIWindow;

class LIXmlLayoutBuilder
{
public:
    LIXmlLayoutBuilder(){}
    virtual ~LIXmlLayoutBuilder(){}
    virtual QLayout* build(
            const QDomElement&      _element, 
            const LIApplication&    _app,
            LIWindow&               _window) = 0;
};

#endif // LIXMLLAYOUTBUILDER_H

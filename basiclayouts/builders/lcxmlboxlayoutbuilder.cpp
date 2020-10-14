#include "lcxmlboxlayoutbuilder.h"
#include "LIApplication.h"
#include "LIXmlWidgetBuilder.h"

#include <QDomNode>
#include <QBoxLayout>
#include <QDebug>


//==============================================================================
static const struct
{
    struct
    {
        QString name = "dir";
        struct 
        {
            QString reverse = "reverse";
            QString forward = "forward";
        }vals;
    }dir;

    QString spacing = "spacing";
    QString value = "value";

}__sAttributes;

//------------------------------------------------------------------------------
static const struct
{
    QString widgets     = "widgets";
    QString layout      = "layout";
    QString spacing     = "spacing"; //Отступ между двумя элементами.
    QString stretch     = "stretch"; //Растяжка.
}__sTags;

//==============================================================================
LCXmlBoxLayoutBuilder::LCXmlBoxLayoutBuilder(EOrientation _orient) : 
    mOrientation(_orient)
{

}

//------------------------------------------------------------------------------
LCXmlBoxLayoutBuilder::~LCXmlBoxLayoutBuilder()
{
}

//------------------------------------------------------------------------------
static void buildLayout(
        QBoxLayout* _layout, 
        const QDomElement &_element,
        const LIApplication& _app);

//------------------------------------------------------------------------------
QLayout* LCXmlBoxLayoutBuilder::build(
        const QDomElement& _element, 
        const LIApplication& _app)
{

    QBoxLayout* layout;
    QString dir = _element.attribute(__sAttributes.dir.name);

    switch(mOrientation)
    {
    case EOrientation::HORIZONTAL:
        if(dir == __sAttributes.dir.vals.reverse)
        {
            layout = new QBoxLayout(QBoxLayout::Direction::RightToLeft);
            buildLayout( layout, _element, _app); 
        }
        else 
        {
            layout = new QBoxLayout(QBoxLayout::Direction::LeftToRight);
            buildLayout( layout, _element, _app); 
        }
        break;

    case EOrientation::VERTICAL:
        if(dir == __sAttributes.dir.vals.reverse)
        {
            layout = new QBoxLayout(QBoxLayout::Direction::BottomToTop);
            buildLayout( layout, _element, _app); 
        }
        else 
        {
            layout = new QBoxLayout(QBoxLayout::Direction::TopToBottom);
            buildLayout( layout, _element, _app); 
        }
        break;

    default:
        layout = new QVBoxLayout;
        break;
    }
    return layout; 
}

//------------------------------------------------------------------------------
static void addLayout(
        QBoxLayout* _layout, 
        const QDomElement& _element, 
        const LIApplication& _app);

//------------------------------------------------------------------------------
static void addWidgets(
        QBoxLayout* _layout, 
        const QDomElement& _element, 
        const LIApplication& _app);

//------------------------------------------------------------------------------
static void addSpacing(
        QBoxLayout* _layout, 
        const QDomElement& _element); 

//------------------------------------------------------------------------------
static void buildLayout(
        QBoxLayout* _layout, 
        const QDomElement &_element,
        const LIApplication& _app)
{
    int spacing = 0;
    QString attr = _element.attribute(__sAttributes.spacing);

    if(!attr.isNull())
    {
        spacing = attr.toInt();
        _layout->setSpacing(spacing);
    }

    int index = 0;
    for(    QDomNode childNode = _element.firstChild(); 
            !childNode.isNull(); 
            childNode = childNode.nextSiblingElement())
    {
        qDebug() << "QBoxLayout build layout pass = " << index;
        index++;
        if(!childNode.isElement()) continue;

        QDomElement el = childNode.toElement();

        if(el.tagName() == __sTags.layout)
        {
            addLayout(_layout, el, _app);
        }
        else if(el.tagName() == __sTags.widgets)
        {
            addWidgets(_layout, el, _app);
        }
        else if(el.tagName() == __sTags.spacing)
        {
            qDebug() << "QBoxLayout add spacing";
            addSpacing(_layout, el);
        }
        else if(el.tagName() == __sTags.stretch)
        {
            qDebug() << "QBoxLayout add stretch";
            _layout->addStretch();
        } 
    }
}

//==============================================================================
static void addLayout(
        QBoxLayout* _layout, 
        const QDomElement& _element, 
        const LIApplication& _app)
{
    for(    QDomNode childNode = _element.firstChild(); 
            !childNode.isNull(); 
            childNode = childNode.nextSiblingElement())
    {
        if(!childNode.isElement()) continue;
        auto el  = childNode.toElement();
        auto builder = _app.getLayoutBuilder(el.tagName());

        if(builder.isNull()) continue;

        auto newlayout = builder->build(el, _app);

        if(newlayout)
        {
            _layout->addLayout(newlayout);
        }
    }
}

//==============================================================================
static void addWidgets(
        QBoxLayout* _layout, 
        const QDomElement& _element, 
        const LIApplication& _app)
{
    QDomNode childNode = _element.firstChild();

    for(    QDomNode childNode = _element.firstChild(); 
            !childNode.isNull(); 
            childNode = childNode.nextSiblingElement())
    {
        if(!childNode.isElement()) continue;
        auto el  = childNode.toElement();
        auto builder = _app.getWidgetBuilder(el.tagName());
        if(builder.isNull()) continue;
        auto widget = builder->build(el, _app);
        if(widget)
        {
            _layout->addWidget(widget);
        }
    }
}

//==============================================================================
static void addSpacing(
        QBoxLayout* _layout, 
        const QDomElement& _element)
{
    QString attr = _element.attribute(__sAttributes.value);
    if(attr.isNull()) return;
    int value = 0;
    bool flag_convert = false;
    value = attr.toInt(&flag_convert);
    if(!flag_convert) return;
    _layout->addSpacing(value);
}

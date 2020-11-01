#include "lcxmlboxlayoutbuilder.h"
#include "LIApplication.h"
#include "LIXmlWidgetBuilder.h"
#include "LIWindow.h"

#include <QDomNode>
#include <QBoxLayout>
#include <QDebug>
#include "common.h"
//==============================================================================
const LCXmlBoxLayoutBuilder::SAttributes LCXmlBoxLayoutBuilder::mAttributes;
const LCXmlBoxLayoutBuilder::STags LCXmlBoxLayoutBuilder::mTags;

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
        const LIApplication& _app,
        LIWindow& _window);

//------------------------------------------------------------------------------
QLayout* LCXmlBoxLayoutBuilder::build(
        const QDomElement&      _element, 
        const LIApplication&    _app,
        LIWindow&               _window)
{

    QBoxLayout* layout;
    QString dir = _element.attribute(mAttributes.dir.name);

    switch(mOrientation)
    {
    case EOrientation::HORIZONTAL:
        if(dir == mAttributes.dir.vals.reverse)
        {
            layout = new QBoxLayout(QBoxLayout::Direction::RightToLeft);
            buildLayout( layout, _element, _app, _window); 
        }
        else 
        {
            layout = new QBoxLayout(QBoxLayout::Direction::LeftToRight);
            buildLayout( layout, _element, _app, _window); 
        }
        break;

    case EOrientation::VERTICAL:
        if(dir == mAttributes.dir.vals.reverse)
        {
            layout = new QBoxLayout(QBoxLayout::Direction::BottomToTop);
            buildLayout( layout, _element, _app, _window); 
        }
        else 
        {
            layout = new QBoxLayout(QBoxLayout::Direction::TopToBottom);
            buildLayout( layout, _element, _app, _window); 
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
        const LIApplication& _app,
        LIWindow& _window);

//------------------------------------------------------------------------------
static void addWidgets(
        QBoxLayout* _layout, 
        const QDomElement& _element, 
        const LIApplication& _app,
        LIWindow& _window);


//------------------------------------------------------------------------------
static void addSpacing(
        QBoxLayout* _layout, 
        const QDomElement& _element); 

//------------------------------------------------------------------------------
static void buildLayout(
        QBoxLayout* _layout, 
        const QDomElement &_element,
        const LIApplication& _app,
        LIWindow& _window)
{
    int spacing = 0;
    QString attr = _element.attribute(LCXmlBoxLayoutBuilder::mAttributes.spacing);

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
        index++;
        if(!childNode.isElement()) continue;

        QDomElement el = childNode.toElement();

        if(el.tagName() == LCXmlBoxLayoutBuilder::mTags.layout)
        {
            addLayout(_layout, el, _app, _window);
        }
        else if(el.tagName() == LCXmlBoxLayoutBuilder::mTags.widgets)
        {
            addWidgets(_layout, el, _app, _window);
        }
        else if(el.tagName() == LCXmlBoxLayoutBuilder::mTags.spacing)
        {
            addSpacing(_layout, el);
        }
        else if(el.tagName() == LCXmlBoxLayoutBuilder::mTags.stretch)
        {
            _layout->addStretch();
        } 
    }
}

//==============================================================================
static void addLayout(
        QBoxLayout* _layout, 
        const QDomElement& _element, 
        const LIApplication& _app,
        LIWindow& _window)
{
    for(    QDomNode childNode = _element.firstChild(); 
            !childNode.isNull(); 
            childNode = childNode.nextSiblingElement())
    {
        if(!childNode.isElement()) continue;
        auto el  = childNode.toElement();
        auto builder = _app.getLayoutBuilder(el.tagName());

        if(builder.isNull()) continue;

        auto newlayout = builder->build(el, _app, _window);

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
        const LIApplication& _app,
        LIWindow& _window)
{
    QDomNode childNode = _element.firstChild();

    QString attr_align = _element.attribute(CCommonAttributes::mAligns.attrName);
    quint16 align = 0;
    if(!attr_align.isNull())
    {
        align = CCommonAttributes::mAligns.toFlags(attr_align);
    }

    for(    QDomNode childNode = _element.firstChild(); 
            !childNode.isNull(); 
            childNode = childNode.nextSiblingElement())
    {
        if(!childNode.isElement()) continue;
        auto el  = childNode.toElement();
        auto builder = _app.getWidgetBuilder(el.tagName());
        if(builder.isNull()) continue;
        auto widget = builder->build(el, _app, _window);
        if(widget)
        {
            if(align == 0)
            {
                _layout->addWidget(widget);
            }
            else
            {
                _layout->addWidget(widget, 0, static_cast<Qt::AlignmentFlag>(align));
            }
        }
    }
}

//==============================================================================
static void addSpacing(
        QBoxLayout* _layout, 
        const QDomElement& _element)
{
    QString attr = _element.attribute(LCXmlBoxLayoutBuilder::mAttributes.value);
    if(attr.isNull()) return;
    int value = 0;
    bool flag_convert = false;
    value = attr.toInt(&flag_convert);
    if(!flag_convert) return;
    _layout->addSpacing(value);
}

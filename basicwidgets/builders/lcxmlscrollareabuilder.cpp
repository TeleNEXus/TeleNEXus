#include "lcxmlscrollareabuilder.h"
#include "LIApplication.h"
#include "LIXmlLayoutBuilder.h"
#include <QScrollArea>
#include <QDomElement>
#include <QFile>
#include <QDebug>
#include "builderscommon.h"

//------------------------------------------------------------------------------
static const struct
{
    QString widgets = "widgets";
} __slTags;

//------------------------------------------------------------------------------
static const struct
{
    QString file = "file";
} __attrName;

//==============================================================================
LCXmlScrollAreaBuilder::LCXmlScrollAreaBuilder()
{

}

//------------------------------------------------------------------------------
LCXmlScrollAreaBuilder::~LCXmlScrollAreaBuilder()
{

}

//------------------------------------------------------------------------------
static QWidget* buildLocal(
        const QDomElement& _element, 
        const LIApplication& _app);

//------------------------------------------------------------------------------
QWidget* LCXmlScrollAreaBuilder::build( const QDomElement& _element, 
                                    const LIApplication& _app)
{

    QString attr;

    attr = _element.attribute(__attrName.file);

    if(!attr.isNull())
    {
        QDomElement el = _app.getDomDocument(attr).documentElement();
        if(!el.isNull())
        {
            if(el.tagName() == _element.tagName()) return build(el, _app);
        } 
    }

    return buildLocal(_element, _app);
}

//------------------------------------------------------------------------------
static QWidget* buildLocal(
        const QDomElement& _element, 
        const LIApplication& _app)
{
    QWidget* widget = nullptr;
    qDebug() << "Scroll area build 0";

    for(QDomNode node = _element.firstChild();
            !node.isNull();
            node = node.nextSibling())
    {
        QDomElement el = node.toElement();
        if(el.isNull()) continue;
        auto builder = _app.getWidgetBuilder(el.tagName());
        if(builder.isNull()) continue;
        auto w = builder->build(el, _app);
        if(w)
        {
            widget = w;
            break;
        }
        else
        {
            continue;
        }
    }

    QScrollArea *scrollarea = new QScrollArea;
    if(widget)
    {
        scrollarea->setWidget(widget);
    }
    else
    {
        scrollarea->setWidget(new QWidget);
    }

    LCWidgetBuildersCommon::initPosition(_element, *scrollarea);
    LCWidgetBuildersCommon::initSize(_element, *scrollarea);
    LCWidgetBuildersCommon::initFixedSize(_element, *scrollarea);

    return scrollarea;
}


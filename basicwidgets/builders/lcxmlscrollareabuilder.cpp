#include "lcxmlwidgetbuilder.h"
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
LCXmlWidgetBuilder::LCXmlWidgetBuilder()
{

}

//------------------------------------------------------------------------------
LCXmlWidgetBuilder::~LCXmlWidgetBuilder()
{

}

//------------------------------------------------------------------------------
static QWidget* buildLocal(
        const QDomElement& _element, 
        const LIApplication& _app);

//------------------------------------------------------------------------------
QWidget* LCXmlWidgetBuilder::build( const QDomElement& _element, 
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
static QWidget* buildFromLayout(const QDomElement& _element, 
        const LIApplication& _app);
//------------------------------------------------------------------------------
static QWidget* buildFromWidgets(const QDomElement& _element, 
        const LIApplication& _app);
//------------------------------------------------------------------------------
static QWidget* buildLocal(
        const QDomElement& _element, 
        const LIApplication& _app)
{

    QDomNode  node = _element.firstChildElement(__slTags.layout);
    QWidget* widget = nullptr;

    if(!node.isNull()) 
    {
        widget = buildFromLayout(node.toElement(), _app);
    }
    else
    {
        node = _element.firstChildElement(__slTags.widgets);

        if(!node.isNull()) 
        {
            widget = buildFromWidgets(node.toElement(), _app);
        }
        else
        {
            widget = new QWidget;
        }
    }

    LCWidgetBuildersCommon::initPosition(_element, *widget);
    LCWidgetBuildersCommon::initSize(_element, *widget);
    LCWidgetBuildersCommon::initFixedSize(_element, *widget);

    return widget;
}

//------------------------------------------------------------------------------
static QWidget* buildFromLayout(const QDomElement& _element, 
        const LIApplication& _app)
{
    QWidget* widget = new QWidget;

    for(QDomNode node = _element.firstChild();
            !node.isNull();
            node = node.nextSibling())
    {
         QDomElement el = node.toElement();
         if(!el.isNull())
         {
                    qDebug() << "Widget set Layout 3 " << el.tagName();
             auto builder = _app.getLayoutBuilder(el.tagName());
             if(!builder.isNull())
             {
                    qDebug() << "Widget set Layout 4 " << el.tagName();
                QLayout* layout = (*builder).build(el, _app);
                if(layout)
                {
                    widget->setLayout(layout);
                    qDebug() << "Widget set Layout 5 " << el.tagName();
                    break;
                }
             }                 
         }
    }
    return widget;
}

//------------------------------------------------------------------------------
static QWidget* buildFromWidgets(const QDomElement& _element, 
        const LIApplication& _app)
{
    QWidget* widget = new QWidget;
    for(QDomNode node = _element.firstChild();
            !node.isNull();
            node = node.nextSibling())
    {
         QDomElement el = node.toElement();
         auto builder = _app.getWidgetBuilder(el.tagName());
         QWidget* addwidget = builder->build(el, _app);
         if(addwidget)
         {
             addwidget->setParent(widget);
         }
    }
    return widget;
}

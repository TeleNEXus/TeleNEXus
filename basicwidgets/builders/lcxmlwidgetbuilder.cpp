#include "lcxmlwidgetbuilder.h"
#include "LIApplication.h"
#include "LIXmlLayoutBuilder.h"
#include <QWidget>
#include <QDomElement>

#include <QDebug>

static const struct
{
    QString layout = "layout";
} sl_Tags;

static const struct
{
    QString title = "title";
    QString type  = "type";
} sl_Attr;

//==============================================================================
LCXmlWidgetBuilder::LCXmlWidgetBuilder()
{

}

//------------------------------------------------------------------------------
LCXmlWidgetBuilder::~LCXmlWidgetBuilder()
{

}

//------------------------------------------------------------------------------
QWidget* LCXmlWidgetBuilder::build( const QDomElement& _element, 
                                    const LIApplication& _app)
{
    QWidget* widget = new QWidget;

    QString attr;

    attr = _element.attribute(sl_Attr.title);

    if(!attr.isNull()) widget->setWindowTitle(attr);

    QDomElement layout = _element.elementsByTagName(
                                            sl_Tags.layout).at(0).toElement();

    if(!layout.isNull())
    {
        QString type = layout.attribute(sl_Attr.type);
        if(!type.isNull())
        {
            auto builder = _app.getLayoutBuilder(type);
            if(!builder.isNull())
            {
                widget->setLayout(builder->build(layout, _app));
            }
        }
    }

    return widget;
}


#include "lcxmltabwidgetbuilder.h"
#include "lcbuilderscommon.h"
#include "LIApplication.h"
#include <QTabWidget>
#include <QDomElement>
#include <QDebug>
#include <qdom.h>
#include <qtablewidget.h>

//------------------------------------------------------------------------------
static const struct
{
  QString item = "item";
} __slTags;

//==============================================================================
LCXmlTabWidgetBuilder::LCXmlTabWidgetBuilder()
{
}

//------------------------------------------------------------------------------
LCXmlTabWidgetBuilder::~LCXmlTabWidgetBuilder()
{
}

//------------------------------------------------------------------------------
static void buildTab(
        const QDomElement& _element, 
        QTabWidget* tabwidget, 
        int _tabindex, 
        const LIApplication& _app);

QWidget* LCXmlTabWidgetBuilder::build(
        const QDomElement& _element, 
        const LIApplication& _app)
{
    QString attr_file = _element.attribute(LCBuildersCommon::mAttributes.file);
    if(!attr_file.isNull())
    {
        QDomElement el = _app.getDomDocument(attr_file).documentElement();
        if(!el.isNull())
        {
            return build(el, _app);
        }
        return new QTabWidget;
    }

    QTabWidget* tabwidget = new QTabWidget;

    int index = 0;

    for( QDomNode node = _element.firstChildElement(__slTags.item);
            !node.isNull();
            node = node.nextSiblingElement(__slTags.item))
    {
        buildTab(node.toElement(), tabwidget, index, _app); 
        index++;
    }

    QString style = LCBuildersCommon::getBaseStyleSheet(_element, _app);
    tabwidget->setStyleSheet(style);

    return tabwidget;
}

//------------------------------------------------------------------------------
static void buildTab(
        const QDomElement& _element, 
        QTabWidget* _tabwidget, 
        int _tabindex, 
        const LIApplication& _app)
{
    QString attr_label = 
      _element.attribute(LCBuildersCommon::mAttributes.label);

    if(attr_label.isNull())
    {
        attr_label = QString("Tab %1").arg(_tabindex);
    }

    QDomNodeList childs = _element.childNodes(); 
    for (int i = 0; i < childs.count(); i++)
    {
        QDomElement element = childs.at(i).toElement();
        if(!element.isNull())
        {
            QSharedPointer<LIXmlWidgetBuilder> builder = 
                _app.getWidgetBuilder(element.tagName());
            if(!builder.isNull())
            {
                QWidget* widget = (*builder).build(
                        childs.at(i).toElement(), _app);
                if (widget)
                {
                    _tabwidget->addTab( widget, attr_label);
                    return;
                }
            }
        }
    }
    _tabwidget->addTab( new QWidget, attr_label);
}

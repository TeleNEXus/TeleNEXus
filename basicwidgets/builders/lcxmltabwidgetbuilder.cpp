#include "lcxmltabwidgetbuilder.h"
#include "LIApplication.h"
#include "LIWindow.h"
#include <QTabWidget>
#include <QDomElement>
#include <QDebug>
#include <qdom.h>
#include <qtablewidget.h>

//------------------------------------------------------------------------------
static const struct
{
    QString label = "label";
    QString icon = "icon";
    QString style = "style"; 
    QString title = "title";
} __attrNames;

//------------------------------------------------------------------------------
static const struct
{
    QString item = "item";
} __elementNames;

//==============================================================================
LCXmlTabWidgetBuilder::LCXmlTabWidgetBuilder()
{
}

//------------------------------------------------------------------------------
LCXmlTabWidgetBuilder::~LCXmlTabWidgetBuilder()
{
}

//------------------------------------------------------------------------------
static void createTab(
        const QDomElement& _element, 
        QTabWidget* tabwidget, 
        int _tabindex, 
        const LIApplication& _app,
        LIWindow& _window);

QWidget* LCXmlTabWidgetBuilder::build(
        const QDomElement& _element, 
        const LIApplication& _app,
        LIWindow& _window)
{
    //TODO: Добавить распознавание стилей.
    QTabWidget* tabwidget = new QTabWidget;

    tabwidget->setWindowTitle(
            _element.attribute(__attrNames.title, "TabWidget"));

    QDomNodeList tabs = _element.childNodes();

    for(int i = 0; i < tabs.size(); i++)
    {
        QDomElement el = tabs.item(i).toElement();
        if(!el.isNull()) 
            createTab(tabs.item(i).toElement(), tabwidget, i, _app, _window); 
    }
    return tabwidget;
}

//------------------------------------------------------------------------------
static void createTab(
        const QDomElement& _element, 
        QTabWidget* _tabwidget, 
        int _tabindex, 
        const LIApplication& _app,
        LIWindow& _window)
{
    //TODO: Добавить иконки.
    QString attr_label = _element.attribute(__attrNames.label);

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
                        childs.at(i).toElement(), _app, _window);
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

#include "lcxmltabwidgetbuilder.h"
#include "LIApplication.h"
#include <QTabWidget>
#include <QDomElement>
#include <QDebug>
#include <qdom.h>
#include <qtablewidget.h>

//------------------------------------------------------------------------------
static const struct
{
    struct
    {
        QString attr = "pos";
        struct
        {
            QString top     = "Top";
            QString bottom  = "Bottom";
            QString left    = "Left";
            QString right   = "Right";
        }vals;
    }position;

    QString file = "file";

} __slAttrs;

//------------------------------------------------------------------------------
static const struct
{
    struct
    {
        QString tag = "item";
        struct
        {
            QString label   = "label";
            QString icon    = "icon";
        }attrs;
    }item;

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
static void createTab(
        const QDomElement& _element, 
        QTabWidget* tabwidget, 
        int _tabindex, 
        const LIApplication& _app);

QWidget* LCXmlTabWidgetBuilder::build(
        const QDomElement& _element, 
        const LIApplication& _app)
{
    QString attr_file = _element.attribute(__slAttrs.file);
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

    /* QString attr_pos = _element.attribute(__slAttrs.position.attr); */

    /* if(!attr_pos.isNull()) */
    /* { */
    /*     if(attr_pos == __slAttrs.position.vals.top) */
    /*     { */
    /*         tabwidget->setTabPosition(QTabWidget::TabPosition::North); */
    /*     } */
    /*     else if(attr_pos == __slAttrs.position.vals.bottom) */
    /*     { */
    /*         tabwidget->setTabPosition(QTabWidget::TabPosition::South); */
    /*     } */
    /*     else if(attr_pos == __slAttrs.position.vals.left) */
    /*     { */
    /*         tabwidget->setTabPosition(QTabWidget::TabPosition::West); */
    /*     } */
    /*     else if(attr_pos == __slAttrs.position.vals.right) */
    /*     { */
    /*         tabwidget->setTabPosition(QTabWidget::TabPosition::East); */
    /*     } */
    /* } */

    int index = 0;

    for( QDomNode node = _element.firstChildElement(__slTags.item.tag);
            !node.isNull();
            node = node.nextSiblingElement(__slTags.item.tag))
    {
        createTab(node.toElement(), tabwidget, index, _app); 
        index++;
    }

    return tabwidget;
}

//------------------------------------------------------------------------------
static void createTab(
        const QDomElement& _element, 
        QTabWidget* _tabwidget, 
        int _tabindex, 
        const LIApplication& _app)
{
    //TODO: Добавить иконки.
    QString attr_label = _element.attribute(__slTags.item.attrs.label);

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

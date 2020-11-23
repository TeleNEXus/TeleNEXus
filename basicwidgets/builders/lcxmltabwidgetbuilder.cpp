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
        int _iconSize,
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
    int icon_size = -1;

    QString attr = _element.attribute(LCBuildersCommon::mAttributes.iconsize);

    if(!attr.isNull())
    {
      bool flag = 0;
      icon_size = attr.toInt(&flag);
      if(flag)
      {
        tabwidget->setIconSize(QSize(icon_size, icon_size));
      }
      else
      {
        icon_size = -1;
      }
    }

    for( QDomNode node = _element.firstChildElement(__slTags.item);
            !node.isNull();
            node = node.nextSiblingElement(__slTags.item))
    {
        buildTab(node.toElement(), tabwidget, index, icon_size, _app); 
        index++;
    }

    QString style = LCBuildersCommon::getBaseStyleSheet(_element, _app);

    tabwidget->setStyleSheet(".QTabBar {" + style + " } ");

    LCBuildersCommon::initPosition(_element, *tabwidget);

    return tabwidget;
}

//------------------------------------------------------------------------------
static void buildTab(
        const QDomElement& _element, 
        QTabWidget* _tabwidget, 
        int _tabindex, 
        int _iconSize,
        const LIApplication& _app)
{
    QString attr_label = 
      _element.attribute(LCBuildersCommon::mAttributes.label);

    QIcon icon;

    QString attr_icon = _element.attribute(LCBuildersCommon::mAttributes.icon);
    if(!attr_icon.isNull()) 
    {
      //Зогрузка изображения.
      QPixmap pixmap = LCBuildersCommon::getPixmap(attr_icon, _app);
      //Масштабирование изобразения под заданные размеры.
      if(_iconSize >= 0)
      {
        int maxsize = (pixmap.width() > pixmap.height()) ? 
          (pixmap.width()) : (pixmap.height());

        float scale = 1;

        if(maxsize > 0)
        {
          scale = (float) _iconSize /(float)  maxsize;
        }

        pixmap = pixmap.scaled(
            pixmap.width() * scale, pixmap.height() * scale);
      }
      if(!pixmap.isNull()) icon.addPixmap(pixmap);
    }


    if(attr_label.isNull() && icon.isNull())
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
                    _tabwidget->addTab( widget, icon, attr_label);
                    return;
                }
            }
        }
    }

    _tabwidget->addTab( new QWidget, icon, attr_label);
}

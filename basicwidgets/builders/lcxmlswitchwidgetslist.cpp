
#include "lcxmlswitchwidgetslist.h"
#include "LIApplication.h"
#include "lcbuilderscommon.h"
#include <QDomElement>
#include <QDebug>
#include <QListWidget>
#include <QSplitter>
#include <QStackedWidget>

//------------------------------------------------------------------------------
static const struct
{
  QString item = "item";
} __slTags;

//==============================================================================
LCXmlSwitchWidgetsListBuilder::LCXmlSwitchWidgetsListBuilder()
{
}

//------------------------------------------------------------------------------
LCXmlSwitchWidgetsListBuilder::~LCXmlSwitchWidgetsListBuilder()
{
}

//------------------------------------------------------------------------------
static QWidget* createWidget(
        const QDomElement& _element, 
        const LIApplication& _app);

static QListWidgetItem* createItem(
    const QDomElement& _element, 
    const LIApplication& _app,
    int   _iconSize = -1);

QWidget* LCXmlSwitchWidgetsListBuilder::build(
        const QDomElement& _element, 
        const LIApplication& _app)
{
    QString attr = _element.attribute(LCBuildersCommon::mAttributes.file);

    //Рекурсивная загрузка виджета из файлов.
    if(!attr.isNull()) 
    {
        QDomDocument doc = _app.getDomDocument(attr);
        if(!doc.isNull()) 
        {
            QDomElement el = doc.documentElement();
            if(!el.isNull())
            {
                if(el.tagName() == _element.tagName()) 
                    return this->build(el, _app);
            }
        }
    }

    QSplitter* splitter = new QSplitter(Qt::Orientation::Horizontal);
    QListWidget* listWidget = new QListWidget;
    QStackedWidget* stacked_widget  = new QStackedWidget;

    QString style_sheet = 
      LCBuildersCommon::getBaseStyleSheet(_element, _app);
    
    style_sheet = QString(".QListWidget { %1 }").arg(style_sheet);

    int icon_size = -1;
    attr = _element.attribute(LCBuildersCommon::mAttributes.iconsize);
    if(!attr.isNull())
    {
      bool flag = 0;
      icon_size = attr.toInt(&flag);
      if(flag)
      {
        listWidget->setIconSize(QSize(icon_size, icon_size));
      }
      else
      {
        icon_size = -1;
      }
    }

    listWidget->setStyleSheet(style_sheet);
    /* listWidget->style()->unpolish(listWidget); */
    /* listWidget->style()->polish(listWidget); */
    /* listWidget->update(); */
    
    for( QDomNode node = _element.firstChildElement(__slTags.item);
            !node.isNull();
            node = node.nextSiblingElement(__slTags.item))
    {
        QDomElement el = node.toElement();

        QWidget* widget = createWidget(el, _app);

        if(widget)
        { 
            listWidget->addItem(createItem(el, _app, icon_size));
            stacked_widget->addWidget(widget);
        }
    }

    splitter->addWidget(listWidget);
    splitter->addWidget(stacked_widget);
    

    QObject::connect(
            listWidget, 
            &QListWidget::currentRowChanged, 
            [stacked_widget](int i)
            {
            stacked_widget->setCurrentIndex(i);
            });
    return splitter;
}

//------------------------------------------------------------------------------
static QWidget* createWidget(
        const QDomElement& _element, 
        const LIApplication& _app)
{
    QWidget* widget = nullptr;
    for(QDomNode node = _element.firstChild();
            !node.isNull();
            node = node.nextSibling())
    {
        QDomElement el = node.toElement();
        if(el.isNull()) continue;

        auto builder = _app.getWidgetBuilder(el.tagName());
        if(builder.isNull()) continue;

        widget = builder->build(el, _app);
    }

    return widget;
}

//------------------------------------------------------------------------------
static QListWidgetItem* createItem(
    const QDomElement& _element, 
    const LIApplication& _app,
    int _iconSize)
{
  QListWidgetItem *item = new QListWidgetItem;

  QString attr = _element.attribute(LCBuildersCommon::mAttributes.text);
  if(!attr.isNull()) item->setText(attr);

  attr = _element.attribute(LCBuildersCommon::mAttributes.icon);
  if(!attr.isNull()) 
  {
    QPixmap pixmap = LCBuildersCommon::getPixmap(attr, _app);
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
    item->setIcon(pixmap);
  }

  return item;
}



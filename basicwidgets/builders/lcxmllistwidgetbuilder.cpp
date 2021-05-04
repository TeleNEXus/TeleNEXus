#include "lcxmllistwidgetbuilder.h"
#include "lcqlistwidget.h"
#include "LIApplication.h"
#include "lcbuilderscommon.h"
#include "lcqlistwidget.h"
#include <QDomElement>

//------------------------------------------------------------------------------
static const struct
{
  QString text = "text";
  QString dataValue = "dataValue";
}__slAttributes;

static const struct
{
  QString item = "item";
} __slTags;

//==============================================================================
LCXmlListWidgetBuilder::LCXmlListWidgetBuilder()
{
}

LCXmlListWidgetBuilder::~LCXmlListWidgetBuilder()
{
}

//------------------------------------------------------------------------------
QWidget* LCXmlListWidgetBuilder::buildLocal( 
    QSharedPointer<SBuildData> _buildData)
{

  auto ret_wrong = [](){return new QListWidget;};

  const QDomElement& element = _buildData->element;
  const LIApplication& app = _buildData->application;

  auto source = [&element, &app]()
  {
    QString attr = element.attribute(LCBuildersCommon::mAttributes.source);
    if(attr.isNull()) return QSharedPointer<LIRemoteDataSource>();
    return app.getDataSource(attr);
  }();

  if(source.isNull()) return ret_wrong();

  QString data = element.attribute(LCBuildersCommon::mAttributes.data);
  if(data.isNull()) return ret_wrong();

  auto format = [&element, &app]() 
  {
    QString attr = element.attribute(
        LCBuildersCommon::mAttributes.dataformatter);
    auto ret = app.getStdDataFormatter(attr);
    if(ret.isNull())
    {
      ret = app.getDataFormatter(attr);
    }
    return ret;
  }();

  if(format.isNull()) return ret_wrong();

  auto list_widget = new LCQListWidget(source, data, format);

  int icon_size = -1;

  QString attr = element.attribute(LCBuildersCommon::mAttributes.iconsize);
  if(!attr.isNull())
  {
    bool flag = 0;
    icon_size = attr.toInt(&flag);
    if(flag)
    {
      list_widget->setIconSize(QSize(icon_size, icon_size));
    }
    else
    {
      icon_size = -1;
    }
  }

  QString style_sheet = 
    LCBuildersCommon::getBaseStyleSheet(element, app);

  list_widget->setStyleSheet(style_sheet);

  auto add_item = [&app, &list_widget](const QDomNode& _node, int _iconSize)
  {

    if(!_node.isElement()) return;
    QDomElement el = _node.toElement();

    QString attr_text = el.attribute(__slAttributes.text);
    if(attr_text.isNull()) return;

    QString attr_val = el.attribute(__slAttributes.dataValue);
    if(attr_val.isNull()) return;


    QListWidgetItem *item = new QListWidgetItem;
    item->setText(attr_text);

    QString attr = el.attribute(LCBuildersCommon::mAttributes.icon);
    if(!attr.isNull()) 
    {
      QPixmap pixmap = LCBuildersCommon::getPixmap(attr, app);
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
    list_widget->addItem(item, attr_val);
  };

  for(QDomNode node = element.firstChildElement(__slTags.item);
      !node.isNull();
      node = node.nextSiblingElement(__slTags.item))
  {
    add_item(node, icon_size);
  }

  return list_widget;
}

#include "lcxmllistwidgetbuilder.h"
#include "LIApplication.h"
#include "lcbuilderscommon.h"
#include <QListWidget>
#include <QDomElement>

//------------------------------------------------------------------------------
static const struct
{
  QString text = "text";
  QString value = "value";
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

  auto ret_wrong = [](){return nullptr;};
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
    QString attr = element.attribute(LCBuildersCommon::mAttributes.dataformatter);
    auto ret = app.getStdDataFormatter(attr);
    if(ret.isNull())
    {
      ret = app.getDataFormatter(attr);
    }
    return ret;
  }();
  if(format.isNull()) return ret_wrong();

  auto list_widget = new QListWidget();

  //load items
  auto add_item = [list_widget](const QDomNode& _node)
  {
    if(!_node.isElement()) return;
    QDomElement el = _node.toElement();

    QString attr_text = el.attribute(__slAttributes.text);
    if(attr_text.isNull()) return;

    QString attr_val = el.attribute(__slAttributes.value);
    if(attr_val.isNull()) return;




    QListWidgetItem *item = new QListWidgetItem;





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
  };

  for(QDomNode node = element.firstChildElement(__slTags.item);
      !node.isNull();
      node = node.nextSiblingElement(__slTags.item))
  {
    add_item(node);
  }

  return nullptr;
}

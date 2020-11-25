#include "lcxmlscrollareabuilder.h"
#include "lcbuilderscommon.h"

#include "LIApplication.h"
#include "LIXmlLayoutBuilder.h"

#include <QScrollArea>
#include <QDomElement>
#include <QDebug>

//==============================================================================
LCXmlScrollAreaBuilder::LCXmlScrollAreaBuilder()
{

}

//------------------------------------------------------------------------------
LCXmlScrollAreaBuilder::~LCXmlScrollAreaBuilder()
{

}

//------------------------------------------------------------------------------
QWidget* LCXmlScrollAreaBuilder::buildLocal(
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
    QWidget* w = builder->build(el, _app);

    if(w)
    {
      widget = w;
      break;
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

  QString style = LCBuildersCommon::getBaseStyleSheet(_element, _app);
  style = QString(".QScrollArea { %1 }").arg(style);
  scrollarea->setStyleSheet(style);

  return scrollarea;
}



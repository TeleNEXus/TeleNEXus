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
      QSharedPointer<SBuildData> _buildData)
{
  const QDomElement& element = _buildData->element;
  const LIApplication& app = _buildData->application;
  QWidget* widget = nullptr;

  for(QDomNode node = element.firstChild();
      !node.isNull();
      node = node.nextSibling())
  {
    QDomElement el = node.toElement();
    if(el.isNull()) continue;
    auto builder = app.getWidgetBuilder(el.tagName());
    if(builder.isNull()) continue;
    QWidget* w = builder->build(el, app);

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

  QString style = LCBuildersCommon::getBaseStyleSheet(element, app);
  style = QString(".QScrollArea { %1 }").arg(style);
  scrollarea->setStyleSheet(style);

  return scrollarea;
}



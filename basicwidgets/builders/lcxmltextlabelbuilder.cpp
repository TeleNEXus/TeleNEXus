#include "lcxmltextlabelbuilder.h"
#include <QLabel>
#include <QDomElement>
#include "builderscommon.h"
#include "LIApplication.h"
#include <QDebug>
//==============================================================================
LCXmlTextLabelBuilder::LCXmlTextLabelBuilder()
{

}

//------------------------------------------------------------------------------
LCXmlTextLabelBuilder::~LCXmlTextLabelBuilder()
{

}

//------------------------------------------------------------------------------
QWidget* LCXmlTextLabelBuilder::build(
    const QDomElement& _element, 
    const LIApplication& _app)
{
  Q_UNUSED(_app);

  QLabel* label = new QLabel();

  //Задание текста в виде атрибута.
  if(!_element.attribute("text").isNull())
  {
    label->setText(_element.attribute("text"));
  }

  QString style;
  style = LCWidgetBuildersCommon::getBaseStyleSheet(_element, _app);
  label->setStyleSheet(style);

  LCWidgetBuildersCommon::initPosition(_element, *label);

  return label;
}

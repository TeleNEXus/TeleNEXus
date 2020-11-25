#include "lcxmltextlabelbuilder.h"
#include "lcbuilderscommon.h"
#include "LIApplication.h"
#include <QLabel>
#include <QDomElement>
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
QWidget* LCXmlTextLabelBuilder::buildLocal(
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
  style = LCBuildersCommon::getBaseStyleSheet(_element, _app);
  label->setStyleSheet(style);

  LCBuildersCommon::initPosition(_element, *label);

  return label;
}

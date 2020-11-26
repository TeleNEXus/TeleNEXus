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
      QSharedPointer<SBuildData> _buildData) 
{

  const QDomElement& element = _buildData->element;
  const LIApplication& app = _buildData->application;
  QLabel* label = new QLabel();

  //Задание текста в виде атрибута.
  if(!element.attribute("text").isNull())
  {
    label->setText(element.attribute("text"));
  }

  QString style;
  style = LCBuildersCommon::getBaseStyleSheet(element, app);
  label->setStyleSheet(style);

  LCBuildersCommon::initPosition(element, *label);

  return label;
}

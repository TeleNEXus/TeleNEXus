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

  QString style = ".QLabel{";
  QString font_style;
  QString attr = _element.attribute(LCWidgetBuildersCommon::mAttributes.fontId);
  if(!attr.isNull())
  {
    attr = _app.getFontStyle(attr);
    if(!attr.isNull())
    {
      font_style = "font: " + attr + ";";
    }
  }
  else
  {
    QString attr = _element.attribute(LCWidgetBuildersCommon::mAttributes.font);
    if(!attr.isNull())
    {
      font_style = "font: " + attr + ";";
    }
  }

  if(!font_style.isNull()) style += font_style;

  attr = _element.attribute(LCWidgetBuildersCommon::mAttributes.colortext);
  if(!attr.isNull())
  {
    style += QString("color: %1;").arg(attr);
  }

  attr = _element.attribute(LCWidgetBuildersCommon::mAttributes.colorbg);
  if(!attr.isNull())
  {
    style = style + QString("background: %1;").arg(attr);
  }
  style += "}";
  label->setStyleSheet(style);

  LCWidgetBuildersCommon::initPosition(_element, *label);

  return label;
}

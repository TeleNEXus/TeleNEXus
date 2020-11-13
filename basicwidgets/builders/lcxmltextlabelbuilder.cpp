#include "lcxmltextlabelbuilder.h"
#include <QLabel>
#include <QDomElement>
#include "builderscommon.h"
#include "LIApplication.h"
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
  else
  {
    //Задание текста в виде узла.
    QDomNode node = _element.firstChild();
    while(!node.isNull())
    {
      if(node.isText())
      {
        label->setText(node.nodeValue());
      }
      node = node.nextSibling();
    }
  }

  QString attr = _element.attribute(LCWidgetBuildersCommon::mAttributes.fontId);
  QFont font;
  if(!attr.isNull())
  {
    bool flag = false;
    QFont font = _app.getFont(attr, &flag);
    if(flag)
    {
      label->setFont(font);
    }
  }
  else
  {

    attr = _element.attribute(LCWidgetBuildersCommon::mAttributes.fontname);
    if(!attr.isNull())
    {
      font.setFamily(attr);
      label->setFont(font);
    }

    attr = _element.attribute(LCWidgetBuildersCommon::mAttributes.fontsize);
    if(!attr.isNull())
    {
      bool flag = false;
      int fs = attr.toInt(&flag);
      if(flag)
      {
        font.setPointSize(fs);
        label->setFont(font);
      }
    }
  }

  attr = _element.attribute(LCWidgetBuildersCommon::mAttributes.colorbg);
  if(!attr.isNull())
  {
    QColor color = LCWidgetBuildersCommon::attributeToColor(attr);
    if(color.isValid())
    {
      QPalette pal = label->palette();
      label->setAutoFillBackground(true);
      pal.setColor(QPalette::ColorRole::Background, color);
      label->setPalette(pal);
    }
  }

  attr = _element.attribute(LCWidgetBuildersCommon::mAttributes.colortext);
  if(!attr.isNull())
  {
    QColor color = LCWidgetBuildersCommon::attributeToColor(attr);
    if(color.isValid())
    {
      QPalette pal = label->palette();
      label->setAutoFillBackground(true);
      pal.setColor(QPalette::ColorRole::Foreground, color);
      label->setPalette(pal);
    }
  }

  LCWidgetBuildersCommon::initPosition(_element, *label);

  return label;
}

#include "lcxmlbuttonbuilder.h"
#include <QPushButton>
//==============================================================================
static const struct 
{
  QString text = "text";
}__slAttributes;

static const struct
{
  struct
  {
    QString tagsName = "data";
    struct
    {
      QString source = "sourceName";
      QString dataName = "dataName";
      QString value   = "value";
    }attributes;
  }data;
}__slTags;

//==============================================================================
LCXmlButtonBuilder::LCXmlButtonBuilder(EType _type) : mType(_type)
{
}

//------------------------------------------------------------------------------
LCXmlButtonBuilder::~LCXmlButtonBuilder()
{
}

//------------------------------------------------------------------------------
static QPushButton* buildWriteButton(
    const QDomElement& _element, const LIApplication& _app);

static QPushButton* buildControllWindowButton(
    const QDomElement& _element, const LIApplication& _app);

QWidget* LCXmlButtonBuilder::build( const QDomElement& _element, 
    const LIApplication& _app)
{
  switch(mType)
  {
  case EType::writeData:
    return buildWriteButton(_element, _app);

  case EType::controlWindows:
    return buildControllWindowButton(_element, _app);
  default:
    return new QPushButton();
  }
}

//==============================================================================
static QPushButton* buildWriteButton(
    const QDomElement& _element, const LIApplication& _app)
{
}

//==============================================================================
static QPushButton* buildControllWindowButton(
    const QDomElement& _element, const LIApplication& _app)
{
}



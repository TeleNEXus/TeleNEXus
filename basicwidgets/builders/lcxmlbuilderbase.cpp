#include "lcxmlbuilderbase.h"
#include "lcbuilderscommon.h"

#include "LIApplication.h"
#include <QWidget>

#include <QDomElement>

//==============================================================================
LCXmlBuilderBase::LCXmlBuilderBase()
{
}

//------------------------------------------------------------------------------
LCXmlBuilderBase::~LCXmlBuilderBase()
{
}

//------------------------------------------------------------------------------
QWidget* LCXmlBuilderBase::build( const QDomElement& _element, 
    const LIApplication&    _app)
{
  QString attr_file = _element.attribute(LCBuildersCommon::mAttributes.file);
  if(!attr_file.isNull())
  {
    QDomElement el = _app.getDomDocument(attr_file).documentElement();
    if(el.isNull()) return nullptr;
    if(el.tagName() != _element.tagName()) return nullptr;
    return build(el, _app);
  }
  return buildLocal(_element, _app);
}


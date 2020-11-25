#include "lcxmlremlabelbuilder.h"
#include "lcqremlabel.h"
#include "lcbuilderscommon.h"
#include "lcxmlstddataformatterfactory.h"
#include "LIApplication.h"

#include <QDomElement>
#include <QDebug>


//==============================================================================
LCXmlRemLabelBuilder::LCXmlRemLabelBuilder()
{

}

//------------------------------------------------------------------------------
LCXmlRemLabelBuilder::~LCXmlRemLabelBuilder()
{

}

//------------------------------------------------------------------------------
QWidget* LCXmlRemLabelBuilder::buildLocal(const QDomElement& _element, 
    const LIApplication& _app)
{
  QLabel* ret = nullptr;
  QString data;
  QString attr = _element.attribute(LCBuildersCommon::mAttributes.source);
  QSharedPointer<LIRemoteDataSource> source;
  QSharedPointer<LCStringDataFormatterBase> format;

  if(attr.isNull())
  {
    goto LABEL_WRONG_EXIT;
  }

  source = _app.getDataSource(attr);

  if(source.isNull())
  {
    goto LABEL_WRONG_EXIT;
  }

  data = _element.attribute(LCBuildersCommon::mAttributes.data);

  if(data.isNull())
  {
    goto LABEL_WRONG_EXIT;
  }

  format = LCXmlStdDataFormatterFactory::
    instance().createStringFormatter(_element.attributes());

  if(format.isNull())
  {
    goto LABEL_WRONG_EXIT;
  }

  ret = new LCQRemLabel(data, source, format);

LABEL_WRONG_EXIT:

  if(ret == nullptr) ret = new QLabel(_element.tagName());
  QString style = LCBuildersCommon::getBaseStyleSheet(_element, _app);
  if(!style.isNull()) ret->setStyleSheet(".LCQRemLabel { " + style + " }");
  LCBuildersCommon::initPosition(_element, *ret);
  return ret;
}


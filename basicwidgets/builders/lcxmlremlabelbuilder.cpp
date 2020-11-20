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
const struct
{
  QString data    = "data";
  QString source  = "source";
  QString format  = "format";
} __attrNames;

//------------------------------------------------------------------------------
QWidget* LCXmlRemLabelBuilder::build(const QDomElement& _element, 
    const LIApplication& _app)
{
  QLabel* ret = nullptr;
  QString data;
  QString attr = _element.attribute(__attrNames.source);
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

  data = _element.attribute(__attrNames.data);

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
  qDebug() << "LCQRemLabelBuilder style = " << style;
  ret->setStyleSheet(style);

  LCBuildersCommon::initPosition(_element, *ret);
  return ret;
}


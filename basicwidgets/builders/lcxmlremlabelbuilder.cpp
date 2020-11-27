#include "lcxmlremlabelbuilder.h"
#include "lcqremlabel.h"
#include "lcbuilderscommon.h"
#include "lcxmlformatterfactory.h"
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
QWidget* LCXmlRemLabelBuilder::buildLocal(
      QSharedPointer<SBuildData> _buildData) 
{
  const QDomElement& element = _buildData->element;
  const LIApplication& app = _buildData->application;
  QLabel* ret = nullptr;
  QString data;
  QString attr = element.attribute(LCBuildersCommon::mAttributes.source);
  QSharedPointer<LIRemoteDataSource> source;
  QSharedPointer<LIDataFormatter> format;

  if(attr.isNull())
  {
    goto LABEL_WRONG_EXIT;
  }

  source = app.getDataSource(attr);

  if(source.isNull())
  {
    goto LABEL_WRONG_EXIT;
  }

  data = element.attribute(LCBuildersCommon::mAttributes.data);

  if(data.isNull())
  {
    goto LABEL_WRONG_EXIT;
  }

  format = LCXmlStdDataFormatterFactory::
    instance().createStringFormatter(element.attributes());

  if(format.isNull())
  {
    goto LABEL_WRONG_EXIT;
  }

  ret = new LCQRemLabel(data, source, format);

LABEL_WRONG_EXIT:

  if(ret == nullptr) ret = new QLabel(element.tagName());
  QString style = LCBuildersCommon::getBaseStyleSheet(element, app);
  ret->setStyleSheet(style);
  LCBuildersCommon::initPosition(element, *ret);
  return ret;
}


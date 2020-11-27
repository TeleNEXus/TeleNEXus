#include "lcxmlremcomboboxbuilder.h"
#include "lcqremcombobox.h"
#include "lcxmlformatterfactory.h"
#include "lcbuilderscommon.h"
#include "LIApplication.h"
#include "LIWindow.h"

#include <QDomElement>
#include <qcombobox.h>
#include <qdom.h>
#include <qwidget.h>

//==============================================================================
LCXmlRemComboBoxBuilder::LCXmlRemComboBoxBuilder()
{

}

//------------------------------------------------------------------------------
LCXmlRemComboBoxBuilder::~LCXmlRemComboBoxBuilder()
{

}

//------------------------------------------------------------------------------
const struct
{
  QString dataread    = "read";
  QString datawrite   = "write";
  /* QString source      = "source"; */
  QString format      = "format";
  /* QString text        = "text"; */
  QString value       = "value";
} __attrNames;

//------------------------------------------------------------------------------
const struct
{
  QString item    = "item";
} __elementNames;

//------------------------------------------------------------------------------buildCombobox
static void buildCombobox(   const QDomElement& _element, 
    LCQRemComboBox* _box, 
    QSharedPointer<LIDataFormatter> __format);

//------------------------------------------------------------------------------build
QWidget* LCXmlRemComboBoxBuilder::buildLocal(
      QSharedPointer<SBuildData> _buildData)
{
  const QDomElement& element = _buildData->element;
  const LIApplication& app = _buildData->application;

  QWidget *ret = nullptr;
  QString dataread;
  QString datawrite;
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

  dataread = element.attribute(__attrNames.dataread);

  if(dataread.isNull())
  {
    goto LABEL_WRONG_EXIT;
  }

  datawrite = element.attribute(__attrNames.datawrite);

  if(datawrite.isNull())
  {
    datawrite = dataread;
  }

  format = LCXmlStdDataFormatterFactory::instance().
    createStringFormatter(element.attributes());

  if(format.isNull())
  {
    goto LABEL_WRONG_EXIT;
  }

  ret = new LCQRemComboBox(dataread, datawrite, source, format);
  buildCombobox(element, static_cast<LCQRemComboBox*>(ret), format);

LABEL_WRONG_EXIT:
  if(ret == nullptr) 
  {
    ret = new QComboBox(); 
    static_cast<QComboBox*>(ret)->setEnabled(false);
    static_cast<QComboBox*>(ret)->addItem(element.tagName());
  }

  QString style = LCBuildersCommon::getBaseStyleSheet(element, app);
  ret->setStyleSheet(style);

  LCBuildersCommon::initPosition(element, *ret);
  return ret;
}

//------------------------------------------------------------------------------buildCombobox
static void buildCombobox(   const QDomElement& _element, 
    LCQRemComboBox* _box,
    QSharedPointer<LIDataFormatter> _format)
{
  QDomNodeList nodes = _element.childNodes();
  for(int i = 0; i < nodes.length(); i++)
  {
    QDomElement el = nodes.at(i).toElement();
    if(el.isNull()) continue;
    if(el.tagName() != __elementNames.item) continue;

    QString name = el.attribute(LCBuildersCommon::mAttributes.text);
    QString val = el.attribute(__attrNames.value);
    if (val.isNull()) 
    {
      continue;
    }
    if (name == "")
    {
      name = val;
    }

    val = _format->normalize(val);

    if(val.isNull())
    {
      continue;
    }

    _box->addItem(name, val);
  }
}


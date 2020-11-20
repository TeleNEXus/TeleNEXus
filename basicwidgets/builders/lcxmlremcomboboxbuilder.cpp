#include "lcxmlremcomboboxbuilder.h"
#include "lcqremcombobox.h"
#include "lcxmlstddataformatterfactory.h"
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
  QString source      = "source";
  QString format      = "format";
  QString text        = "text";
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
    QSharedPointer<LCStringDataFormatterBase> __format);

//------------------------------------------------------------------------------build
QWidget* LCXmlRemComboBoxBuilder::build(const QDomElement& _element, 
    const LIApplication& _app)
{
  QWidget *ret = nullptr;
  QString dataread;
  QString datawrite;
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

  dataread = _element.attribute(__attrNames.dataread);

  if(dataread.isNull())
  {
    goto LABEL_WRONG_EXIT;
  }

  datawrite = _element.attribute(__attrNames.datawrite);

  if(datawrite.isNull())
  {
    datawrite = dataread;
  }

  format = LCXmlStdDataFormatterFactory::instance().
    createStringFormatter(_element.attributes());

  if(format.isNull())
  {
    goto LABEL_WRONG_EXIT;
  }

  ret = new LCQRemComboBox(dataread, datawrite, source, format);
  buildCombobox(_element, static_cast<LCQRemComboBox*>(ret), format);

LABEL_WRONG_EXIT:
  if(ret == nullptr) 
  {
    ret = new QComboBox(); 
    static_cast<QComboBox*>(ret)->setEnabled(false);
    static_cast<QComboBox*>(ret)->addItem(_element.tagName());
  }

  QString style = LCBuildersCommon::getBaseStyleSheet(_element, _app);
  /* style = QString(".LCQRemComboBox { %1 }").arg(style); */
  ret->setStyleSheet(style);

  /* LCBuildersCommon::initFixedSize(_element, *ret); */
  /* LCBuildersCommon::initSize(_element, *ret); */
  LCBuildersCommon::initPosition(_element, *ret);
  return ret;
}

//------------------------------------------------------------------------------buildCombobox
static void buildCombobox(   const QDomElement& _element, 
    LCQRemComboBox* _box,
    QSharedPointer<LCStringDataFormatterBase> _format)
{
  QDomNodeList nodes = _element.childNodes();
  for(int i = 0; i < nodes.length(); i++)
  {
    QDomElement el = nodes.at(i).toElement();
    if(el.isNull()) continue;
    if(el.tagName() != __elementNames.item) continue;

    QString name = el.attribute(__attrNames.text);
    QString val = el.attribute(__attrNames.value);
    if (val.isNull()) 
    {
      continue;
    }
    if (name == "")
    {
      name = val;
    }

    val = _format->normalizeString(val);

    if(val.isNull())
    {
      continue;
    }

    _box->addItem(name, val);
  }
}

#include "lcxmlremwritebuttonbuilder.h"
#include "lcqremwritebutton.h"
#include <QDomElement>
#include "LIApplication.h"
#include "lcxmlstddataformatterfactory.h"
#include "builderscommon.h"
#include <QWidget>

//==============================================================================
const LCXmlRemWriteButtonBuilder::SAttributes 
LCXmlRemWriteButtonBuilder::mAttributes;

LCXmlRemWriteButtonBuilder::LCXmlRemWriteButtonBuilder()
{
}

LCXmlRemWriteButtonBuilder::~LCXmlRemWriteButtonBuilder()
{
}

//------------------------------------------------------------------------------
QWidget* LCXmlRemWriteButtonBuilder::build(
        const QDomElement& _element, const LIApplication& _app)
{
    QString data_name = _element.attribute(
            CBuildersCommonAttributes::mSourceAttr.dataReadWriteName);
    if(dataName.isNull()) return nullptr;
   
   QString source_name = _element.attribute(
           CBuildersCommonAttributes::mSourceAttr.dataSourceName); 

   auto source = _app.getDataSource(_element.attribute(
               CBuildersCommonAttributes::mSourceAttr.dataSourceName));
   
   if(source.isNull()) return nullptr;

   auto format = LCXmlStdDataFormatterFactory::instance().
       createStringFormatter(_element.attributes());

   if(format.isNull()) return nullptr;

   QString label = _element.attribute(mAttributes.label);

   if(label.isNull())
   {
       label = "WriteButton";
   } 
   
   QByteArray data;
   
   /* return new LCQRemWriteButton(source, data_name, */ 
}


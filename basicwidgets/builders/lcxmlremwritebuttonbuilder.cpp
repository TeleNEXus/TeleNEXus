#include "lcxmlremwritebuttonbuilder.h"
#include "lcqremwritebutton.h"
#include <QDomElement>
#include "LIApplication.h"
#include "lcxmlstddataformatterfactory.h"
//==============================================================================
LCXmlRemWriteButtonBuilder::LCXmlRemWriteButtonBuilder()
{
}
//------------------------------------------------------------------------------

QWidget* LCXmlRemWriteButtonBuilder::build(
        const QDomElement& _element, const LIApplication& _app)
{
    QString attr_dataName   = _element.attribute(mAttributes.dataName);
    QString attr_data       = _element.attribute(mAttributes.data);
    QString attr_format     = _element.attribute(mAttributes.format);
    if(attr_dataName.isNull() || 
            attr_data.isNull() || 
            attr_format.isNull()) return nullptr;
}


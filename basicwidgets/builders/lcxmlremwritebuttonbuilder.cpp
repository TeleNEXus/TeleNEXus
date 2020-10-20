#include <QDomElement>
#include <QWidget>

#include "LIApplication.h"
#include "lcxmlstddataformatterfactory.h"
#include "lcxmlremwritebuttonbuilder.h"
#include "lcqremwritebutton.h"

//==============================================================================
const LCXmlRemWriteButtonBuilder::SAttributes 
LCXmlRemWriteButtonBuilder::mAttributes;

const LCXmlRemWriteButtonBuilder::STags
LCXmlRemWriteButtonBuilder::mTags;

//------------------------------------------------------------------------------
LCXmlRemWriteButtonBuilder::LCXmlRemWriteButtonBuilder()
{
}

//------------------------------------------------------------------------------
LCXmlRemWriteButtonBuilder::~LCXmlRemWriteButtonBuilder()
{
}

//------------------------------------------------------------------------------
QWidget* LCXmlRemWriteButtonBuilder::build(
        const QDomElement& _element, const LIApplication& _app)
{
    LCQRemWriteButton* button = 
        new LCQRemWriteButton(_element.attribute(mAttributes.text));

    for(    QDomNode node = _element.firstChild(); 
            !node.isNull(); 
            node = node.nextSibling())
    {
        QDomElement el = node.toElement();
        if(el.isNull()) continue;
        if(el.tagName() != mTags.data.tagsName) continue;
        auto format = LCXmlStdDataFormatterFactory::
            instance().createStringFormatter(el.attributes());
        if(format.isNull()) continue;
        auto source = _app.getDataSource(
                el.attribute(mTags.data.attributes.source));
        if(source.isNull()) continue;
        auto data_name = el.attribute(mTags.data.attributes.dataName);
        if(data_name.isNull()) continue;
        auto value = el.attribute(mTags.data.attributes.value);
        if(value.isNull()) continue;
        auto data = format->toBytes(value);
        if(data.isNull()) continue;
        button->addDataWrite(source, data_name, data);
    } 
    return button;
}


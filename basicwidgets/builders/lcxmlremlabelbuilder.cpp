#include "lcxmlremlabelbuilder.h"
#include "lcqremlabel.h"
#include "LIApplication.h"
#include "LIWindow.h"
#include "lcxmlstddataformatterfactory.h"

#include <QDomElement>


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
        const LIApplication& _app,
        LIWindow& _window)
{
    Q_UNUSED(_window);
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

    format = LCXmlStdDataFormatterFactory::instance().createStringFormatter(_element.attributes());

    if(format.isNull())
    {
        goto LABEL_WRONG_EXIT;
    }

    return new LCQRemLabel(data, source, format);

LABEL_WRONG_EXIT:
    return new QLabel(_element.tagName());
}

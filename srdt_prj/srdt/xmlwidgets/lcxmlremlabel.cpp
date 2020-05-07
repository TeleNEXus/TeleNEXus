#include "lcxmlremlabel.h"

#include "lcqremlabel.h"
#include "lcxmlremotedatasourcemap.h"
#include "lcxmlstddataformatterfactory.h"

using namespace remgui;

namespace xmlwidgetcreators
{

LCXmlRemLabel::LCXmlRemLabel()
{

}

LCXmlRemLabel::~LCXmlRemLabel()
{

}

const struct
{
    QString data    = "data";
    QString source  = "source";
    QString format  = "format";
} __attrNames;

//----------------------------------------------------------------------------------------------------------------------
QWidget* LCXmlRemLabel::create(const QDomElement& _element)
{

    QString data;
    QString attr = _element.attribute(__attrNames.source);
    QSharedPointer<LIRemoteDataSource> source;
    QSharedPointer<LCStringDataFormatterBase> format;

    if(attr.isNull())
    {
        goto LABEL_WRONG_EXIT;
    }

    source = LCXmlRemoteDataSourceMap::instace().getRemoteDataSorce(attr);

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

} //namespace

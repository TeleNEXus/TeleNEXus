#include "lcxmlremlineedit.h"
#include "lcqremlineedit.h"
#include "lcxmlremotedatasourcemap.h"
#include "lcxmlstddataformatterfactory.h"

using namespace remgui;

namespace xmlwidgetcreators
{

LCXmlRemLineEdit::LCXmlRemLineEdit()
{

}

LCXmlRemLineEdit::~LCXmlRemLineEdit()
{

}

const struct
{
    QString data    = "data";
    QString source  = "source";
    QString format  = "format";
} __attrNames;

QWidget* LCXmlRemLineEdit::create(const QDomElement& _element)
{
    QString data;
    QString attr = _element.attribute(__attrNames.source);
    QSharedPointer<LCRemoteDataSourceInterface> source;
    QSharedPointer<LCDataStrFormatBase> format;

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

    format = LCXmlStdDataFormatterFactory::instance().create(_element.attributes());

    if(format.isNull())
    {
        goto LABEL_WRONG_EXIT;
    }

    return new LCQRemLineEdit(data, source, format);

LABEL_WRONG_EXIT:
    return new QLineEdit(_element.tagName());
}

} //namespace

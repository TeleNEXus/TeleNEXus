#include "lcxmlremlineedit.h"
#include "lcqremlineedit.h"
#include "lcxmlremotedatasourcemap.h"
#include "lcxmlstddataformatterfactory.h"

using namespace remgui;

namespace xmlwidgetcreators
{

LCXmlRemLineEdit::LCXmlRemLineEdit() : mpOrderCtrl(nullptr)
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
    QWidget* ret = nullptr;

    QString data;
    QString attr = _element.attribute(__attrNames.source);
    QSharedPointer<LCRemoteDataSourceInterface> source;
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

    ret= new LCQRemLineEdit(data, source, format);
    return ret;

LABEL_WRONG_EXIT:

    ret= new QLineEdit(_element.tagName());
    return ret;
}

} //namespace

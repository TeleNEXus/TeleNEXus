#include "lcxmlremotedatalineeditbuilder.h"
#include "lcqremlineedit.h"
#include "LIApplication.h"
#include "lcxmlstddataformatterfactory.h"
#include <QDomElement>

//======================================================================================================================
LCXmlRemoteDataLineEditBuilder::LCXmlRemoteDataLineEditBuilder()
{

}

//----------------------------------------------------------------------------------------------------------------------
LCXmlRemoteDataLineEditBuilder::~LCXmlRemoteDataLineEditBuilder()
{

}

//----------------------------------------------------------------------------------------------------------------------
const struct
{
    QString data    = "data";
    QString source  = "source";
    QString format  = "format";
} __attrNames;

//----------------------------------------------------------------------------------------------------------------------
QWidget* LCXmlRemoteDataLineEditBuilder::build(const QDomElement& _element, const LIApplication& _app)
{
    QWidget* ret = nullptr;

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

    ret= new LCQRemLineEdit(data, source, format);
    return ret;

LABEL_WRONG_EXIT:

    ret= new QLineEdit(_element.tagName());
    return ret;
}

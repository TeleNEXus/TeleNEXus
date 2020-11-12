#include "lcxmlremlineeditbuilder.h"
#include "lcqremlineedit.h"
#include "LIApplication.h"
#include "lcxmlstddataformatterfactory.h"
#include <QDomElement>
#include "builderscommon.h"

//==============================================================================
LCXmlRemLineEditBuilder::LCXmlRemLineEditBuilder()
{

}

//------------------------------------------------------------------------------
LCXmlRemLineEditBuilder::~LCXmlRemLineEditBuilder()
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
QWidget* LCXmlRemLineEditBuilder::build(const QDomElement& _element, 
        const LIApplication& _app)
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

LABEL_WRONG_EXIT:

    if(ret == nullptr) ret= new QLineEdit(_element.tagName());
    LCWidgetBuildersCommon::initSize(_element, *ret);
    LCWidgetBuildersCommon::initFixedSize(_element, *ret);
    LCWidgetBuildersCommon::initPosition(_element, *ret);
    return ret;
}


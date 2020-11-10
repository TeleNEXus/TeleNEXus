#include "lcxmlremcombolabelbuilder.h"
#include "lcqremcombolabel.h"
#include "lcxmlstddataformatterfactory.h"
#include "LIApplication.h"

#include <QDomElement>
#include <qdom.h>
#include <qwidget.h>

#include <QDebug>
#include "builderscommon.h"
//==============================================================================
LCXmlRemComboLabelBuilder::LCXmlRemComboLabelBuilder()
{

}

//------------------------------------------------------------------------------
LCXmlRemComboLabelBuilder::~LCXmlRemComboLabelBuilder()
{

}

//------------------------------------------------------------------------------
const struct
{
    QString dataread    = "read";
    QString source      = "source";
    QString format      = "format";
    QString text        = "text";
    QString value       = "value";
    QString movie       = "movie";
    QString picture     = "picture";
} __attrNames;

//------------------------------------------------------------------------------
const struct
{
    QString item    = "item";
} __elementNames;

//------------------------------------------------------------------------------
static void buildComboLabel( const QDomElement& _element, 
                        LCQRemComboLabel* _label,
                        QSharedPointer<LCStringDataFormatterBase> _format,
                        const LIApplication& _app);

//------------------------------------------------------------------------------
QWidget* LCXmlRemComboLabelBuilder::build(const QDomElement& _element, 
                                            const LIApplication& _app)
{
    LCQRemComboLabel *remlabel= nullptr;
    QString dataread;
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

    format = LCXmlStdDataFormatterFactory::instance().
                                createStringFormatter(_element.attributes());
    if(format.isNull())
    {
        goto LABEL_WRONG_EXIT;
    }

    remlabel = new LCQRemComboLabel(dataread, source, format);

    buildComboLabel(_element, remlabel, format, _app);

LABEL_WRONG_EXIT:
    QLabel* ret = remlabel;
    if(ret == nullptr) ret = new QLabel(_element.tagName());

    LCWidgetBuildersCommon::initSize(_element, *ret);
    LCWidgetBuildersCommon::initFixedSize(_element, *ret);
    LCWidgetBuildersCommon::initPosition(_element, *ret);

    return ret;
}

//------------------------------------------------------------------------------
static void buildComboLabel( const QDomElement& _element, 
                        LCQRemComboLabel* _label,
                        QSharedPointer<LCStringDataFormatterBase> _format,
                        const LIApplication& _app)
{
    for( QDomNode node = _element.firstChildElement(__elementNames.item);
            !node.isNull();
            node = node.nextSiblingElement(__elementNames.item))
    {
        QDomElement el = node.toElement();
        QString attr_value = el.attribute(__attrNames.value);

        if(!attr_value.isNull())
        {
            attr_value = _format->normalizeString(attr_value);
            if(attr_value.isNull()) continue;
        }

        QString attr_item = el.attribute(__attrNames.text);

        if(!attr_item.isNull())
        {
            _label->addItem(attr_item, attr_value);
            continue;
        }

        attr_item = el.attribute(__attrNames.movie);

        if(!attr_item.isNull())
        {
            _label->addItem(
                    LCWidgetBuildersCommon::getMovie(attr_item, _app), 
                    attr_value);
            continue;
        }

        attr_item = el.attribute(__attrNames.picture);

        if(!attr_item.isNull())
        {
            _label->addItem(
                    LCWidgetBuildersCommon::getPixmap(attr_item, _app), 
                    attr_value);
            continue;
        }
    }
}

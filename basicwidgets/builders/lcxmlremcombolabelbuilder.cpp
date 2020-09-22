#include "lcxmlremcombolabelbuilder.h"
#include "lcqremcombolabel.h"
#include "lcxmlstddataformatterfactory.h"
#include "LIApplication.h"

#include <QDomElement>
#include <qdom.h>
#include <qwidget.h>

#include <QDebug>
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
    QString name        = "name";
    QString value       = "value";
} __attrNames;

//------------------------------------------------------------------------------
const struct
{
    QString item    = "item";
} __elementNames;

//------------------------------------------------------------------------------
static void buildLabel( const QDomElement& _element, 
                        LCQRemComboLabel* _label,
                        QSharedPointer<LCStringDataFormatterBase> _format);

//------------------------------------------------------------------------------
QWidget* LCXmlRemComboLabelBuilder::build(const QDomElement& _element, 
                                            const LIApplication& _app)
{
    QWidget *ret;
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
        qDebug() << "LCQRemComboLabel-------------------------no format";
        goto LABEL_WRONG_EXIT;
    }

    ret = new LCQRemComboLabel(dataread, source, format);

    buildLabel(_element, static_cast<LCQRemComboLabel*>(ret), format);
    return ret;

LABEL_WRONG_EXIT:
    ret = new QLabel(_element.tagName());
    static_cast<QLabel*>(ret)->setEnabled(false);
    return ret;
}

//------------------------------------------------------------------------------
static void buildLabel( const QDomElement& _element, 
                        LCQRemComboLabel* _label,
                        QSharedPointer<LCStringDataFormatterBase> _format)
{
    QDomNodeList elements = _element.elementsByTagName(__elementNames.item);
    for(int i = 0; i < elements.length(); i++)
    {
        QString name = elements.at(i).toElement().attribute(__attrNames.name);
        QString val = elements.at(i).toElement().attribute(__attrNames.value);
        if (val.isNull()) 
        {
          continue;
        }

        if (name == "")
        {
            name = val;
        }

        val = _format->normalizeString(val);

        if(val.isNull())
        {
            continue;
        }

        _label->addItem(name, val);
    }
}

#include "lcxmlremcomboboxbuilder.h"
#include "lcqremcombobox.h"
#include "lcxmlstddataformatterfactory.h"
#include "LIApplication.h"

#include <QDomElement>
#include <qcombobox.h>
#include <qdom.h>
#include <qwidget.h>

//==============================================================================
LCXmlRemComboBoxBuilder::LCXmlRemComboBoxBuilder()
{

}

//------------------------------------------------------------------------------
LCXmlRemComboBoxBuilder::~LCXmlRemComboBoxBuilder()
{

}

//------------------------------------------------------------------------------
const struct
{
    QString dataread    = "read";
    QString datawrite   = "write";
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

//------------------------------------------------------------------------------buildBox
static void buildBox(   const QDomElement& _element, 
                        LCQRemComboBox* _box, 
                        QSharedPointer<LCStringDataFormatterBase> __format);

//------------------------------------------------------------------------------build
QWidget* LCXmlRemComboBoxBuilder::build(const QDomElement& _element, 
                                            const LIApplication& _app)
{
    QWidget *ret;
    QString dataread;
    QString datawrite;
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

    datawrite = _element.attribute(__attrNames.datawrite);
    
    if(datawrite.isNull())
    {
        datawrite = dataread;
    }

    format = LCXmlStdDataFormatterFactory::instance().
                                createStringFormatter(_element.attributes());

    if(format.isNull())
    {
        goto LABEL_WRONG_EXIT;
    }
    
    ret = new LCQRemComboBox(dataread, datawrite, source, format);
    buildBox(_element, static_cast<LCQRemComboBox*>(ret), format);

    return ret;

LABEL_WRONG_EXIT:
    ret = new QComboBox();
    static_cast<QComboBox*>(ret)->setEnabled(false);
    static_cast<QComboBox*>(ret)->addItem(_element.tagName());
    return ret;
}

//------------------------------------------------------------------------------buildBox
static void buildBox(   const QDomElement& _element, 
                        LCQRemComboBox* _box,
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

        _box->addItem(name, val);
    }
}

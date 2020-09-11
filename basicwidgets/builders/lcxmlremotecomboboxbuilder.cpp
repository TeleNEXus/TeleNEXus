#include "lcxmlremotecomboboxbuilder.h"
#include "lcqremcombobox.h"
#include "LIApplication.h"
#include "lcxmlstddataformatterfactory.h"

#include <QDomElement>
#include <qcombobox.h>
#include <qdom.h>
#include <qwidget.h>


//==============================================================================
LCXmlRemoteComboBoxBuilder::LCXmlRemoteComboBoxBuilder()
{

}

//------------------------------------------------------------------------------
LCXmlRemoteComboBoxBuilder::~LCXmlRemoteComboBoxBuilder()
{

}

//------------------------------------------------------------------------------
const struct
{
    QString dataread    = "read";
    QString datawrite   = "write";
    QString source      = "source";
    QString format      = "format";
    QString note        = "note";
    QString value       = "value";
} __attrNames;

const struct
{
    QString item    = "item";
} __elementNames;

static void buildBox(const QDomElement& _element, LCQRemComboBox* _box);
//------------------------------------------------------------------------------
QWidget* LCXmlRemoteComboBoxBuilder::build(const QDomElement& _element, 
                                            const LIApplication& _app)
{
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

    LCQRemComboBox* rbox;

    if(format.isNull())
    {
        goto LABEL_WRONG_EXIT;
    }
    
    rbox = new LCQRemComboBox(dataread, datawrite, source, format);
    buildBox(_element, rbox);

    return rbox;

LABEL_WRONG_EXIT:
    QComboBox* ret = new QComboBox();
    ret->addItem(_element.tagName());
    return ret;
}

static void buildBox(const QDomElement& _element, LCQRemComboBox* _box)
{
    QDomNodeList elements = _element.elementsByTagName(__elementNames.item);
    for(int i = 0; i < elements.length(); i++)
    {
        QString note = elements.at(i).toElement().attribute(__attrNames.note);
        QString val = elements.at(i).toElement().attribute(__attrNames.value);
        if((note == "") || (val == "")) continue;
        _box->addItem(note, val);
    }
}

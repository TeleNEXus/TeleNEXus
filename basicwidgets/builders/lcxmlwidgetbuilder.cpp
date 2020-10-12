#include "lcxmlwidgetbuilder.h"
#include "LIApplication.h"
#include "LIXmlLayoutBuilder.h"
#include <QWidget>
#include <QDomElement>
#include <QFile>
#include <QDebug>

//------------------------------------------------------------------------------
static const struct
{
    QString layout = "layout";
} __tagsName;

//------------------------------------------------------------------------------
static const struct
{
    QString file = "file";
    QString title = "title";
    QString type  = "type";
} __attrName;

//==============================================================================
LCXmlWidgetBuilder::LCXmlWidgetBuilder()
{

}

//------------------------------------------------------------------------------
LCXmlWidgetBuilder::~LCXmlWidgetBuilder()
{

}
//------------------------------------------------------------------------------
static QWidget* buildWidget(
        const QDomElement& _element, 
        const LIApplication& _app);
static QDomElement loadDomElement(const QString& _fileName);

//------------------------------------------------------------------------------
QWidget* LCXmlWidgetBuilder::build( const QDomElement& _element, 
                                    const LIApplication& _app)
{

    QString attr;

    attr = _element.attribute(__attrName.file);

    if(!attr.isNull())
    {
        QString file = _app.getProjectPath() + attr;
        QDomElement el = loadDomElement(file);
        if(!el.isNull())
        {
            if(el.tagName() == _element.tagName()) return buildWidget(el, _app);
        } 
    }

    return buildWidget(_element, _app);
}

//------------------------------------------------------------------------------
static QWidget* buildWidget(
        const QDomElement& _element, 
        const LIApplication& _app)
{
    QWidget* widget = new QWidget;

    QString attr;

    attr = _element.attribute(__attrName.title);

    if(!attr.isNull()) widget->setWindowTitle(attr);

    QDomElement layout = 
        _element.elementsByTagName(
                __tagsName.layout).at(0).toElement();

    if(!layout.isNull())
    {
        QString type = layout.attribute(__attrName.type);
        if(!type.isNull())
        {
            auto builder = _app.getLayoutBuilder(type);
            if(!builder.isNull())
            {
                widget->setLayout(builder->build(layout, _app));
            }
        }
    }
    return widget;
}

//==============================================================================
static QDomElement loadDomElement(const QString& _fileName)
{
    QFile file(_fileName);

    QDomDocument domDoc;
    QString errorStr;
    int errorLine;
    int errorColumn;

    if(!domDoc.setContent(&file, true, &errorStr, &errorLine, &errorColumn))
    {
        qDebug() << 
            "Parse file "       << file.fileName() << 
            " error at line:"   << errorLine <<
            " column:"          << errorColumn << 
            " msg: "            << errorStr;
    }
    return domDoc.documentElement();
}

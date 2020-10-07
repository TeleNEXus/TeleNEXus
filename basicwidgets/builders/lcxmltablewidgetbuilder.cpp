#include "lcxmltablewidgetbuilder.h"
#include <QTableWidget>
#include <QDomElement>
//==============================================================================
LCXmlTableWidgetBuilder::LCXmlTableWidgetBuilder()
{

}

//------------------------------------------------------------------------------
LCXmlTableWidgetBuilder::~LCXmlTableWidgetBuilder()
{

}

//------------------------------------------------------------------------------
const struct
{
//    QString value       = "value";
} __attrNames;

//------------------------------------------------------------------------------
const struct
{
//    QString item    = "item";
} __elementNames;

//------------------------------------------------------------------------------
QWidget* LCXmlTableWidgetBuilder::build(const QDomElement& _element, const LIApplication& _app)
{
    Q_UNUSED(_element);
    Q_UNUSED(_app);

//    QLabel* label = new QLabel();
//
//    //Задание текста в виде атрибута.
//    if(!_element.attribute("text").isNull())
//    {
//        label->setText(_element.attribute("text"));
//        return label;
//    }
//
//    //Задание текста в виде узла.
//    QDomNode node = _element.firstChild();
//    while(!node.isNull())
//    {
//        if(node.isText())
//        {
//            label->setText(node.nodeValue());
//        }
//        node = node.nextSibling();
//    }
    return new QTableWidget();
}

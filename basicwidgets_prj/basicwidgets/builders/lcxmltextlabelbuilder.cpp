#include "lcxmltextlabelbuilder.h"
#include <QLabel>
#include <QDomElement>
//======================================================================================================================
LCXmlTextLabelBuilder::LCXmlTextLabelBuilder()
{

}

//----------------------------------------------------------------------------------------------------------------------
LCXmlTextLabelBuilder::~LCXmlTextLabelBuilder()
{

}

//----------------------------------------------------------------------------------------------------------------------
QWidget* LCXmlTextLabelBuilder::build(const QDomElement& _element, const LIApplication& _app)
{
    Q_UNUSED(_app);

    QLabel* label = new QLabel();

    //Задание текста в виде атрибута.
    if(!_element.attribute("text").isNull())
    {
        label->setText(_element.attribute("text"));
        return label;
    }

    //Задание текста в виде узла.
    QDomNode node = _element.firstChild();
    while(!node.isNull())
    {
        if(node.isText())
        {
            label->setText(node.nodeValue());
        }
        node = node.nextSibling();
    }
    return label;
}

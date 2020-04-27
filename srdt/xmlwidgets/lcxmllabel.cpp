#include "lcxmllabel.h"

#include <QLabel>

namespace xmlwidgetcreators
{

//======================================================================================================================
LCXmlLabel::LCXmlLabel()
{

}

QWidget* LCXmlLabel::create(const QDomElement& _element)
{
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

} //namespace

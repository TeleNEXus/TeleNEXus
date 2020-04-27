#include "lcxmltestwidgetcreators.h"
#include "lcxmllayoutsfactory.h"

#include <QDebug>

namespace xmltestwidget
{

//======================================================================================================================
LCXmlWidget::LCXmlWidget()
{

}

QWidget* LCXmlWidget::create(const QDomElement& _element)
{
    QWidget* widget = new QWidget;

    QString attr;

    attr = _element.attribute("title");

    if(!attr.isNull()) widget->setWindowTitle(attr);

    QDomNode node = _element.firstChild();
    while(!node.isNull())
    {
        if(!node.isElement()) continue;
        QDomElement element = node.toElement();
        if(!element.isNull())
        {
            QLayout* layout = LCXmlLayoutsFactory::create(element);
            if(layout)
            {
                widget->setLayout(layout);
                return widget;
            }
        }
        node = node.nextSibling();
    }
    return widget;
}

//======================================================================================================================
LCXmlWidgetLabel::LCXmlWidgetLabel()
{

}

QWidget* LCXmlWidgetLabel::create(const QDomElement& _element)
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

}

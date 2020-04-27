#include "lcxmlwidget.h"
#include "lcxmllayoutsfactory.h"

#include <QWidget>

namespace xmlwidgetcreators
{

//======================================================================================================================
LCXmlWidget::LCXmlWidget()
{

}

//----------------------------------------------------------------------------------------------------------------------
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

} //namespace

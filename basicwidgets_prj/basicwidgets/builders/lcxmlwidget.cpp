#include "lcxmlwidget.h"
#include "LIApplication.h"
#include "LIXmlLayoutBuilder.h"
#include <QWidget>
#include <QDomElement>

//======================================================================================================================
LCXmlWidget::LCXmlWidget()
{

}

//----------------------------------------------------------------------------------------------------------------------
QWidget* LCXmlWidget::build(const QDomElement& _element, const LIApplication& _app)
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
            QSharedPointer<LIXmlLayoutBuilder> lb = _app.getLayoutBuilder(element.tagName());
            if(!lb.isNull())
            {
                widget->setLayout(lb->build(element, _app));
            }
        }
        node = node.nextSibling();
    }
    return widget;
}


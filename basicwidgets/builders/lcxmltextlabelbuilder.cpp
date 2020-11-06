#include "lcxmltextlabelbuilder.h"
#include <QLabel>
#include <QDomElement>
#include "builderscommon.h"
//==============================================================================
LCXmlTextLabelBuilder::LCXmlTextLabelBuilder()
{

}

//------------------------------------------------------------------------------
LCXmlTextLabelBuilder::~LCXmlTextLabelBuilder()
{

}

//------------------------------------------------------------------------------
QWidget* LCXmlTextLabelBuilder::build(
        const QDomElement& _element, 
        const LIApplication& _app)
{
    Q_UNUSED(_app);

    QLabel* label = new QLabel();

    //Задание текста в виде атрибута.
    if(!_element.attribute("text").isNull())
    {
        label->setText(_element.attribute("text"));
    }
    else
    {
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
    }

    LCWidgetBuildersCommon::initPosition(_element, *label);

    return label;
}

#include "lcxmlboxlayoutbuilder.h"
#include "LIApplication.h"
#include "LIXmlWidgetBuilder.h"

#include <QDomNode>
#include <QBoxLayout>

//======================================================================================================================
const LCXmlBoxLayoutBuilder::SAttributes LCXmlBoxLayoutBuilder::mAttributes;

LCXmlBoxLayoutBuilder::LCXmlBoxLayoutBuilder() : mFlagDirModeDef(false),
                                                 mDirectionDef(QBoxLayout::Direction::TopToBottom)
{

}

//----------------------------------------------------------------------------------------------------------------------
LCXmlBoxLayoutBuilder::LCXmlBoxLayoutBuilder(QBoxLayout::Direction _direction) :
        mFlagDirModeDef(false),
        mDirectionDef(_direction)
{

}

//----------------------------------------------------------------------------------------------------------------------
LCXmlBoxLayoutBuilder::~LCXmlBoxLayoutBuilder()
{

}

//----------------------------------------------------------------------------------------------------------------------
static QLayout* buildLayout(const QDomElement &_element,QBoxLayout::Direction _direction, const LIApplication& _app);
QLayout* LCXmlBoxLayoutBuilder::build(const QDomElement& _element, const LIApplication& _app)
{
    if(mFlagDirModeDef)
    {
        return buildLayout(_element, mDirectionDef, _app);
    }

    QString attrDir = _element.attribute("direction");

    if(attrDir.isNull())            return  buildLayout(_element, QBoxLayout::Direction::TopToBottom, _app);
    if(attrDir == mAttributes.directionVals.leftToRight)
        return  buildLayout(_element, QBoxLayout::Direction::LeftToRight, _app);
    if(attrDir == mAttributes.directionVals.topToBottom)
        return  buildLayout(_element, QBoxLayout::Direction::TopToBottom, _app);
    if(attrDir == mAttributes.directionVals.rightToLeft)
        return  buildLayout(_element, QBoxLayout::Direction::RightToLeft, _app);
    if(attrDir == mAttributes.directionVals.bottomToTop)
        return  buildLayout(_element, QBoxLayout::Direction::BottomToTop, _app);

    return buildLayout(_element, mDirectionDef, _app);
}

//----------------------------------------------------------------------------------------------------------------------
static QLayout* buildLayout(const QDomElement &_element, QBoxLayout::Direction _direction, const LIApplication& _app)
{
    QDomNode childNode = _element.firstChild();

    QBoxLayout* layout = new QBoxLayout(_direction);

    while(!childNode.isNull())
    {
        if(childNode.isElement())
        {
            QDomElement nodeElement = childNode.toElement();
            if(nodeElement.tagName() == LCXmlLayoutBuilderBase::mCommonTags.layout)
            {
                QDomElement nodeElement = childNode.toElement();
                QLayout* childLayout = nullptr;
                auto builder = _app.getLayoutBuilder(nodeElement.attribute(
                                                  LCXmlLayoutBuilderBase::mCommonLayoutsAttributes.layoutType));
                if(!builder.isNull())
                {
                    childLayout = builder->build(nodeElement, _app);
                }

                if(childLayout)
                {
                    layout->addLayout(childLayout);
                }
                else
                {
                    layout->addItem(new QSpacerItem(0,0));
                }
            }
            else
            {
                QWidget* widget = nullptr;
                auto widgetBuilder = _app.getWidgetBuilder(nodeElement.tagName());

                if(!widgetBuilder.isNull())
                {
                    widget = widgetBuilder->build(nodeElement, _app);
                }

                if(widget)
                {
                    layout->addWidget(widget);
                }
                else
                {
                    layout->addItem(new QSpacerItem(0,0));
                }
            }
        }
        //--------------------------------
        childNode = childNode.nextSibling();
    }
    return layout;
}

#include "lcxmlboxlayoutbuilder.h"
#include "LIApplication.h"
#include "LIXmlWidgetBuilder.h"

#include <QDomNode>
#include <QBoxLayout>

//======================================================================================================================
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
    static const QString attrDirection( "direction"  );
    static const QString dirLeftToRight("LeftToRight");
    static const QString dirRightToLeft("RightToLeft");
    static const QString dirTopToButtom("TopToBottom");
    static const QString dirButtomToTop("BottomToTop");

    if(mFlagDirModeDef)
    {
        return buildLayout(_element, mDirectionDef, _app);
    }

    QString attrDir = _element.attribute("direction");

    if(attrDir.isNull())            return  buildLayout(_element, QBoxLayout::Direction::TopToBottom, _app);
    if(attrDir == dirLeftToRight)   return  buildLayout(_element, QBoxLayout::Direction::LeftToRight, _app);
    if(attrDir == dirTopToButtom)   return  buildLayout(_element, QBoxLayout::Direction::TopToBottom, _app);
    if(attrDir == dirRightToLeft)   return  buildLayout(_element, QBoxLayout::Direction::RightToLeft, _app);
    if(attrDir == dirButtomToTop)   return  buildLayout(_element, QBoxLayout::Direction::BottomToTop, _app);

    return                                  buildLayout(_element, mDirectionDef, _app);
}

//----------------------------------------------------------------------------------------------------------------------
static QLayout* buildLayout(const QDomElement &_element,QBoxLayout::Direction _direction, const LIApplication& _app)
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
                QSharedPointer<LIXmlLayoutBuilder> builder =
                        _app.getLayoutBuilder(nodeElement.attribute(
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
                QSharedPointer<LIXmlWidgetBuilder> wb = _app.getWidgetBuilder(nodeElement.tagName());
                if(!wb.isNull())
                {
                    widget = wb->build(nodeElement, _app);
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

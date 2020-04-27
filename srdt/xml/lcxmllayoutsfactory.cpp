#include "lcxmllayoutsfactory.h"
#include "lcxmlwidgetcreatorbase.h"
#include "lcxmlwidgetcreatorsmap.h"

#include <QDebug>

static const QString __tagNameLayout("layout");
static const QString __tagNameGridLayoutRow("row");
static const QString __tagNameGridLayoutColumn("col");

//======================================================================================================================
class CLayoutCreatorBase
{
public:
    CLayoutCreatorBase(){}
    virtual QLayout* create(const QDomElement &_element) = 0;
};

//======================================================================================================================
class CBoxLayoutCreator
{

public:

    CBoxLayoutCreator()
    {
    }

    static QLayout* create(const QDomElement &_element, QBoxLayout::Direction _direction)
    {
        QDomNode childNode = _element.firstChild();

        QBoxLayout* layout = new QBoxLayout(_direction);

        while(!childNode.isNull())
        {
            if(childNode.isElement())
            {
                QDomElement nodeElement = childNode.toElement();
                if(nodeElement.tagName() == __tagNameLayout)
                {
                    QLayout* childLayout = LCXmlLayoutsFactory::create(nodeElement);
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
                    LCXmlWidgetCreatorBase* widgetCreator =
                                    LCXmlWidgetCreatorsMap::instace().getCreator(nodeElement.tagName());
                    if(widgetCreator)
                    {
                        QWidget* childWidget = widgetCreator->create(nodeElement);
                        if(childWidget)
                        {
                            layout->addWidget(childWidget);
                        }
                        else
                        {
                            layout->addItem(new QSpacerItem(0,0));
                        }
                    }
                }
            }
            //--------------------------------
            childNode = childNode.nextSibling();
        }
        return layout;
    }

    static QLayout* create(const QDomElement &_element)
    {
        static const QString attrDirection("direction");

        static const QString dirLeftToRight("LeftToRight");
        static const QString dirRightToLeft("RightToLeft");
        static const QString dirTopToButtom("TopToBottom");
        static const QString dirButtomToTop("BottomToTop");

        QString attrDir = _element.attribute("direction");

        if(attrDir.isNull())            return  create(_element, QBoxLayout::Direction::TopToBottom);
        if(attrDir == dirLeftToRight)   return  create(_element, QBoxLayout::Direction::LeftToRight);
        if(attrDir == dirTopToButtom)   return  create(_element, QBoxLayout::Direction::TopToBottom);
        if(attrDir == dirRightToLeft)   return  create(_element, QBoxLayout::Direction::RightToLeft);
        if(attrDir == dirButtomToTop)   return  create(_element, QBoxLayout::Direction::BottomToTop);

        return                                  create(_element, QBoxLayout::Direction::TopToBottom);
    }
};

//======================================================================================================================
class CGridLayoutCreator : public CLayoutCreatorBase
{

private:
    quint32 mRow;
    quint32 mColumn;
    //------------------------------------------------------------------------------------------------------------------
    void createRow(const QDomElement &_element)
    {
        quint32 col = 0;

        QDomNode childNode = _element.firstChild();

        if(childNode.isNull())
        {
            mpLayout->addItem(new QSpacerItem(0,0), mRow, col);
            mRow++;
            return;
        }

        while(!childNode.isNull())
        {
            if(childNode.isElement())
            {
                QDomElement nodeElement = childNode.toElement();
                if(nodeElement.tagName() == __tagNameLayout)
                {
                    QLayout* layout = LCXmlLayoutsFactory::create(nodeElement);
                    if(layout)
                    {
                        mpLayout->addLayout(layout, mRow, col);
                    }
                    else
                    {
                        mpLayout->addItem(new QSpacerItem(0,0), mRow, col);
                    }
                }
                else
                {
                    LCXmlWidgetCreatorBase* wcr = LCXmlWidgetCreatorsMap::instace().getCreator(nodeElement.tagName());
                    if(wcr)
                    {
                        QWidget* widget = wcr->create(nodeElement);
                        if(widget)
                        {
                            mpLayout->addWidget(widget, mRow, col);
                        }
                        else
                        {
                            mpLayout->addItem(new QSpacerItem(0,0), mRow, col);
                        }
                    }
                }
                col++;
            }
            //--------------------------------
            childNode = childNode.nextSibling();
        }
        mRow++;
        if(col > mColumn) mColumn = col;
    }

    //------------------------------------------------------------------------------------------------------------------
    void createCol(const QDomElement &_element)
    {
        quint32 row = 0;

        QDomNode childNode = _element.firstChild();

        if(childNode.isNull())
        {
            mpLayout->addItem(new QSpacerItem(0,0), row, mColumn);
            mColumn++;
            return;
        }

        while(!childNode.isNull())
        {
            if(childNode.isElement())
            {
                QDomElement nodeElement = childNode.toElement();
                if(nodeElement.tagName() == __tagNameLayout)
                {
                    QLayout* layout = LCXmlLayoutsFactory::create(nodeElement);
                    if(layout)
                    {
                        mpLayout->addLayout(layout, row, mColumn);
                    }
                    else
                    {
                        mpLayout->addItem(new QSpacerItem(0,0), row, mColumn);
                    }
                }
                else
                {
                    LCXmlWidgetCreatorBase* wcr = LCXmlWidgetCreatorsMap::instace().getCreator(nodeElement.tagName());
                    if(wcr)
                    {
                        QWidget* widget = wcr->create(nodeElement);
                        if(widget)
                        {
                            mpLayout->addWidget(widget, row, mColumn);
                        }
                        else
                        {
                            mpLayout->addItem(new QSpacerItem(0,0), row, mColumn);
                        }
                    }
                }
                row++;
            }
            //--------------------------------
            childNode = childNode.nextSibling();
        }
        mColumn++;
        if(row > mRow) mRow = row;
    }

    QGridLayout* mpLayout;

public:
    CGridLayoutCreator() : mRow(0), mColumn(0)
    {
        mpLayout = new QGridLayout;
    }

    virtual QLayout* create(const QDomElement &_element) override
    {
        QDomNode childNode = _element.firstChild();

        while(!childNode.isNull())
        {
            if(childNode.isElement())
            {
                QDomElement element = childNode.toElement();
                if(element.tagName() == __tagNameGridLayoutRow)
                {
                    createRow(element);
                }
                else if(element.tagName() == __tagNameGridLayoutColumn)
                {
                    createCol(element);

                }
                else
                {
                    //TODO: сформировать сообщение отладки об недопустимом элементе.
                }
            }
            //--------------------------------
            childNode = childNode.nextSibling();
        }
        return mpLayout;
    }
};


//======================================================================================================================
LCXmlLayoutsFactory::LCXmlLayoutsFactory()
{
}

//----------------------------------------------------------------------------------------------------------------------
QLayout* LCXmlLayoutsFactory::create(const QDomElement &_element)
{
    static const QString attrNameLayoutType(    "type"  );
    static const QString layoutTypeGrid(        "grid"  );
    static const QString layoutTypeBox(         "box"   );
    static const QString layoutTypeVBox(        "vbox"  );
    static const QString layoutTypeHBox(        "hbox"  );

    if(_element.tagName() != __tagNameLayout) return nullptr;

    QString layoutType = _element.attribute(attrNameLayoutType);

    if(layoutType.isNull()){
        return nullptr;
    }

    if(layoutType == layoutTypeGrid)
    {
        return CGridLayoutCreator().create(_element);
    }

    if(layoutType == layoutTypeBox)
    {
        return CBoxLayoutCreator::create(_element);
    }

    if(layoutType == layoutTypeHBox)
    {
        return CBoxLayoutCreator::create(_element, QBoxLayout::Direction::LeftToRight);
    }

    if(layoutType == layoutTypeVBox)
    {
        return CBoxLayoutCreator::create(_element, QBoxLayout::Direction::TopToBottom);
    }

    return nullptr;
}

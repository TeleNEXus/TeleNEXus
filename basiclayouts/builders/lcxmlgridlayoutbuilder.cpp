#include "lcxmlgridlayoutbuilder.h"
#include "LIApplication.h"
#include "LIXmlWidgetBuilder.h"
#include "common.h"

#include <QGridLayout>
#include <QSharedPointer>
#include <QDomNode>
#include <QDebug>
#include <qnamespace.h>
#include <QMap>

//==============================================================================
static const struct
{
    QString spacing     = "spacing";
    QString hspacing    = "hspacing";
    QString vspacing    = "vspacing";
    QString value       = "value";
    QString minwidth    = "minwidth";
}__sAttributes;

//------------------------------------------------------------------------------
static const struct
{
    QString row         = "row";
    QString column      = "col";
    QString widgets     = "widgets";
    QString layout      = "layout";
    /* QString spacing     = "spacing"; //Отступ между двумя элементами. */
    QString stretch     = "stretch"; //Растяжка.
}__sTags;

//------------------------------------------------------------------------------
class CBuildData
{
public:
    int mRow = 0;
    int mColumn = 0;
    QGridLayout* mpLayout = nullptr;
    //Карта глобального выравнивания элементов столбцов.
    QMap<int, Qt::AlignmentFlag> mColumnAlign; 
    //Карта глобального выравнивания элементов строк.
    QMap<int, Qt::AlignmentFlag> mRowAlign;
private:
public:
    CBuildData() = delete;
    explicit CBuildData(QGridLayout* _p_layout) : mpLayout(_p_layout){}
};

//==============================================================================
LCXmlGridLayoutBuilder::LCXmlGridLayoutBuilder()
{

}

//------------------------------------------------------------------------------
LCXmlGridLayoutBuilder::~LCXmlGridLayoutBuilder()
{

}

//------------------------------------------------------------------------------
static void buildRow(
        const QDomElement &_element, 
        const LIApplication& _app, 
        CBuildData& _buildData);

static void buildCol(
        const QDomElement &_element, 
        const LIApplication& _app, 
        CBuildData& _buildData);

QLayout* LCXmlGridLayoutBuilder::build(
        const QDomElement& _element, 
        const LIApplication& _app)
{
    qDebug() << "QGridLayout build 0";
    QGridLayout* layout = new QGridLayout;
    CBuildData buildData(layout);

    //Глобальная инициализация компоновщика.
    QString attr = _element.attribute(__sAttributes.spacing);
    if(!attr.isNull())
    {
        bool flag = false;
        int spacing = attr.toInt(&flag);
        if(flag)
        {
            buildData.mpLayout->setSpacing(spacing);
        }
    }

    for(QDomNode node = _element.firstChild(); !node.isNull(); node = node.nextSiblingElement())
    {
        if(!node.isElement()) continue;
        QDomElement el = node.toElement();
        if(el.tagName() == __sTags.row)
        {
            buildRow(el, _app, buildData);
        }
        else if (el.tagName() == __sTags.column)
        {
            buildCol(el, _app, buildData);
        }
    }
    
    return layout;
}

//==============================================================================
static int addRowWidgets(
        const QDomElement &_element,
        CBuildData& _buildData, 
        int _startCol, 
        const LIApplication& _app);

static int addRowLayout(
        const QDomElement &_element,
        CBuildData& _buildData, 
        int _startCol, 
        const LIApplication& _app);

static int addRowStretch(
        const QDomElement &_element,
        CBuildData& _buildData, 
        int _startCol);

//------------------------------------------------------------------------------
static void buildRow(
        const QDomElement &_element, 
        const LIApplication& _app, 
        CBuildData& _buildData)
{
    int curr_col = 0;

    //Анализ глобального выравнивания для элементов строки.
    QString attr_align = 
        _element.attribute(CCommonAttributes::instance().mAligns.attrName);

    if(!attr_align.isNull())
    {
        _buildData.mRowAlign[_buildData.mRow] = 
            CCommonAttributes::instance().mAligns.toFlags(attr_align);
    }

    //Создание элементов строки.
    for( QDomNode node = _element.firstChild(); 
            !node.isNull(); node = node.nextSibling())
    {
        if(!node.isElement()) continue;

        QDomElement el =  node.toElement();

        if(el.tagName() == __sTags.widgets)
        {
            curr_col += addRowWidgets(el, _buildData, curr_col, _app);
        }
        else if(el.tagName() == __sTags.layout)
        {
            curr_col += addRowLayout(el, _buildData, curr_col, _app);
        }
        else if(el.tagName() == __sTags.stretch)
        {
            curr_col += addRowStretch(el, _buildData, curr_col);
        }
    }

    if(curr_col > _buildData.mColumn) _buildData.mColumn = curr_col;
    _buildData.mRow++;
}

//==============================================================================
static int addRowWidgets(
        const QDomElement &_element,
        CBuildData &_buildData, 
        int _startCol, 
        const LIApplication& _app)
{
    int col = _startCol;

    quint16 align_flags = 0; 
    //Анализ атрибута выравнивания элемента.
    QString attr = _element.attribute(
                    CCommonAttributes::instance().mAligns.attrName);
    if(!attr.isNull())
    {
        //Если для элемента установлен атрибут выравнивания,
        //производим его расшифровку.
        align_flags = CCommonAttributes::instance().mAligns.toFlags(attr);
    }
    else if(_buildData.mRowAlign.contains(_buildData.mRow))
    {
        //Если для стобца установлено глобальное выравнивание,
        //используем его.
       align_flags = _buildData.mRowAlign[_buildData.mRow]; 
    }

    for( QDomNode node = _element.firstChild(); 
            !node.isNull(); 
            node = node.nextSibling() )
    {
        QDomElement el = node.toElement();
        if(el.isNull()) continue;
        auto builder = _app.getWidgetBuilder(el.tagName());
        if(builder.isNull()) continue;
        auto widget = builder->build(el, _app);
        if(widget == nullptr) continue;

        //Анализ параметров выравнивания для текущего стролбца.
        if(align_flags == 0)
        {
            if(_buildData.mColumnAlign.contains(col))
            {
                align_flags = _buildData.mColumnAlign[col];
            }
            else
            {
                align_flags = Qt::AlignmentFlag::AlignLeft;
            }
        }

        _buildData.mpLayout->addWidget( 
                widget, 
                _buildData.mRow, 
                col, 
                static_cast<Qt::AlignmentFlag>(align_flags)); 
        col++;
    }
    return col;
}

//==============================================================================
static int addRowLayout(
        const QDomElement &_element,
        CBuildData &_buildData, 
        int _startCol, 
        const LIApplication& _app)
{
    for(    QDomNode node = _element.firstChild(); 
            !node.isNull(); 
            node = node.nextSibling())
    {
        if(!node.isElement()) continue;
        QDomElement el = node.toElement();
        auto builder = _app.getLayoutBuilder(el.tagName());
        if(builder.isNull()) continue;
        auto layout = builder->build(el, _app);
        if(layout == nullptr) continue;
        _buildData.mpLayout->addLayout(layout, _buildData.mRow, _startCol);
        return 1;
    }
    return 0; 
}

//==============================================================================
static int addRowStretch(
        const QDomElement &_element,
        CBuildData &_buildData, 
        int _startCol)
{
    QString attr_val = _element.attribute(__sAttributes.value);
    bool flag = false;
    int val = attr_val.toInt(&flag);

    if(flag)
    {
        _buildData.mpLayout->setColumnStretch(_startCol, val);
    }
    else
    {
        _buildData.mpLayout->setColumnStretch(_startCol, 1);
    }

    return 0;
}




//==============================================================================
static void buildCol(
        const QDomElement &_element, 
        const LIApplication& _app, 
        CBuildData& _buildData)
{
    Q_UNUSED(_element);
    Q_UNUSED(_app);
    Q_UNUSED(_buildData);
    /* quint32 row = 0; */

    /* QDomNode childNode = _element.firstChild(); */

    /* if(childNode.isNull()) */
    /* { */
    /*     _buildData.mpLayout-> */
    /*         addItem(new QSpacerItem(0,0), row, _buildData.mColumn); */

    /*     _buildData.mColumn++; */
    /*     return; */
    /* } */

    /* while(!childNode.isNull()) */
    /* { */
    /*     if(childNode.isElement()) */
    /*     { */
    /*         QDomElement nodeElement = childNode.toElement(); */
    /*         if(nodeElement.tagName() == */ 
    /*                 LCXmlLayoutBuilderBase::mCommonTags.layout) */
    /*         { */
    /*             QLayout* layout = nullptr; */

    /*             QSharedPointer<LIXmlLayoutBuilder> */ 
    /*                 builder = */
    /*                 _app.getLayoutBuilder(nodeElement.attribute( */
    /*                             LCXmlLayoutBuilderBase:: */
    /*                             mCommonLayoutsAttributes.layoutType)); */

    /*             if(!builder.isNull()) */
    /*             { */
    /*                 layout = builder->build(nodeElement, _app); */
    /*             } */

    /*             if(layout) */
    /*             { */
    /*                 _buildData.mpLayout-> */
    /*                     addLayout(layout, row, _buildData.mColumn); */
    /*             } */
    /*             else */
    /*             { */
    /*                 _buildData.mpLayout-> */
    /*                     addItem(new QSpacerItem(0,0), row, _buildData.mColumn); */
    /*             } */
    /*         } */
    /*         else */
    /*         { */
    /*             QWidget* widget = nullptr; */

    /*             QSharedPointer<LIXmlWidgetBuilder> wb = */ 
    /*                 _app.getWidgetBuilder(nodeElement.tagName()); */

    /*             if(!wb.isNull()) */
    /*             { */
    /*                 widget = wb->build(nodeElement, _app); */
    /*             } */

    /*             if(widget) */
    /*             { */
    /*                 _buildData.mpLayout-> */
    /*                     addWidget(widget, row, _buildData.mColumn); */
    /*             } */
    /*             else */
    /*             { */
    /*                 _buildData.mpLayout-> */
    /*                     addItem(new QSpacerItem(0,0), row, _buildData.mColumn); */
    /*             } */
    /*         } */
    /*         row++; */
    /*     } */
    /*     //-------------------------------- */
    /*     childNode = childNode.nextSibling(); */
    /* } */
    /* _buildData.mColumn++; */
    /* if(row > _buildData.mRow) _buildData.mRow = row; */
}

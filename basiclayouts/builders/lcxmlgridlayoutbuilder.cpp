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
    QString minheight   = "minheight";
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

//------------------------------------------------------------------------------build
QLayout* LCXmlGridLayoutBuilder::build(
        const QDomElement& _element, 
        const LIApplication& _app)
{
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

    for(    QDomNode node = _element.firstChild(); 
            !node.isNull(); 
            node = node.nextSiblingElement())
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

//==============================================================================buildRow
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

    //Определение минимально заданной высоны строки.
    QString attr_minheight =
        _element.attribute(__sAttributes.minheight);

    if(!attr_minheight.isNull())
    {
        bool flag = false;
        int minheight = attr_minheight.toInt(&flag);
        if(flag)
        {
            _buildData.mpLayout->setRowMinimumHeight(
                    _buildData.mRow, minheight);
        }
    }
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

//==============================================================================addRowWidgets
static int addRowWidgets(
        const QDomElement &_element,
        CBuildData &_buildData, 
        int _startCol, 
        const LIApplication& _app)
{
    int col = 0;

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
                col + _startCol, 
                static_cast<Qt::AlignmentFlag>(align_flags)); 
        col++;
    }
    return col;
}

//==============================================================================addRowLayout
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

//==============================================================================addRowStretch
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

//==============================================================================buildCol
static int addColWidgets(
        const QDomElement &_element,
        CBuildData& _buildData, 
        int _startRow, 
        const LIApplication& _app);

static int addColLayout(
        const QDomElement &_element,
        CBuildData& _buildData, 
        int _startRow, 
        const LIApplication& _app);

static int addColStretch(
        const QDomElement &_element,
        CBuildData& _buildData, 
        int _startRow);

//------------------------------------------------------------------------------
static void buildCol(
        const QDomElement &_element, 
        const LIApplication& _app, 
        CBuildData& _buildData)
{
    int curr_row = 0;

    //Определение минимально заданной высоны строки.
    QString attr_minwidth =
        _element.attribute(__sAttributes.minwidth);

    if(!attr_minwidth.isNull())
    {
        bool flag = false;
        int minwidth = attr_minwidth.toInt(&flag);
        if(flag)
        {
            _buildData.mpLayout->setColumnMinimumWidth(
                    _buildData.mColumn, minwidth);
        }
    }

    //Анализ глобального выравнивания для элементов строки.
    QString attr_align = 
        _element.attribute(CCommonAttributes::instance().mAligns.attrName);

    if(!attr_align.isNull())
    {
        _buildData.mColumnAlign[_buildData.mRow] = 
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
            curr_row += addColWidgets(el, _buildData, curr_row, _app);
        }
        else if(el.tagName() == __sTags.layout)
        {
            curr_row += addColLayout(el, _buildData, curr_row, _app);
        }
        else if(el.tagName() == __sTags.stretch)
        {
            curr_row += addColStretch(el, _buildData, curr_row);
        }
    }

    if(curr_row > _buildData.mRow) _buildData.mRow = curr_row;
    _buildData.mColumn++;
}

//==============================================================================addColWidgets
static int addColWidgets(
        const QDomElement &_element,
        CBuildData &_buildData, 
        int _startRow, 
        const LIApplication& _app)
{
    int row = 0;

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
    else if(_buildData.mColumnAlign.contains(_buildData.mColumn))
    {
        //Если для стобца установлено глобальное выравнивание,
        //используем его.
       align_flags = _buildData.mColumnAlign[_buildData.mColumn]; 
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
            if(_buildData.mRowAlign.contains(row))
            {
                align_flags = _buildData.mRowAlign[row];
            }
            else
            {
                align_flags = Qt::AlignmentFlag::AlignLeft;
            }
        }

        _buildData.mpLayout->addWidget( 
                widget, 
                row + _startRow,
                _buildData.mColumn, 
                static_cast<Qt::AlignmentFlag>(align_flags)); 
        row++;
    }
    return row;
}

//==============================================================================addColLayout
static int addColLayout(
        const QDomElement &_element,
        CBuildData &_buildData, 
        int _startRow, 
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
        _buildData.mpLayout->addLayout(layout, _startRow, _buildData.mColumn);
        return 1;
    }
    return 0; 
}

//==============================================================================addColStretch
static int addColStretch(
        const QDomElement &_element,
        CBuildData &_buildData, 
        int _startRow)
{
    QString attr_val = _element.attribute(__sAttributes.value);
    bool flag = false;
    int val = attr_val.toInt(&flag);

    if(flag)
    {
        _buildData.mpLayout->setRowStretch(_startRow, val);
    }
    else
    {
        _buildData.mpLayout->setRowStretch(_startRow, 1);
    }

    return 0;
}

#include "lcxmlgridlayoutbuilder.h"
#include "LIApplication.h"
#include "LIXmlWidgetBuilder.h"

#include <QGridLayout>
#include <QSharedPointer>
#include <QDomNode>


/* //============================================================================== */
/* static const struct */
/* { */
/*     QString row = "row"; */
/*     QString column = "col"; */
/* }__gridLayoutTags; */

/* //============================================================================== */
/* struct SBuildData */
/* { */
/*     quint32 mRow = 0; */
/*     quint32 mColumn = 0; */
/*     QGridLayout* mpLayout = nullptr; */
/*     SBuildData() : mpLayout(new QGridLayout){} */
/* }; */

/* //============================================================================== */
/* LCXmlGridLayoutBuilder::LCXmlGridLayoutBuilder() */
/* { */

/* } */

/* //------------------------------------------------------------------------------ */
/* LCXmlGridLayoutBuilder::~LCXmlGridLayoutBuilder() */
/* { */

/* } */

/* //------------------------------------------------------------------------------ */
/* static void createRow( */
/*         const QDomElement &_element, */ 
/*         const LIApplication& _app, */ 
/*         SBuildData& _buildData); */

/* static void createCol( */
/*         const QDomElement &_element, */ 
/*         const LIApplication& _app, */ 
/*         SBuildData& _buildData); */

/* //------------------------------------------------------------------------------ */
/* QLayout* LCXmlGridLayoutBuilder::build( */
/*         const QDomElement& _element, */ 
/*         const LIApplication& _app) */
/* { */
/*     SBuildData buildData; */

/*     QDomNode childNode = _element.firstChild(); */

/*     while(!childNode.isNull()) */
/*     { */
/*         if(childNode.isElement()) */
/*         { */
/*             QDomElement element = childNode.toElement(); */
/*             if(element.tagName() == __gridLayoutTags.row) */
/*             { */
/*                 createRow(element, _app, buildData); */
/*             } */
/*             else if(element.tagName() == __gridLayoutTags.column) */
/*             { */
/*                 createCol(element, _app, buildData); */

/*             } */
/*         } */
/*         //-------------------------------- */
/*         childNode = childNode.nextSibling(); */
/*     } */
/*     return buildData.mpLayout; */
/* } */

/* //------------------------------------------------------------------------------ */
/* static void createRow( */
/*         const QDomElement &_element, */ 
/*         const LIApplication& _app, */ 
/*         SBuildData& _buildData) */
/* { */
/*     quint32 col = 0; */

/*     QDomNode childNode = _element.firstChild(); */

/*     if(childNode.isNull()) */
/*     { */
/*         //Если нет элементов для добавления, добавляем пустую строку. */
/*         _buildData.mpLayout-> */
/*             addItem(new QSpacerItem(0,0), _buildData.mRow, col); */
/*         _buildData.mRow++; */
/*         return; */
/*     } */

/*     while(!childNode.isNull()) */
/*     { */
/*         if(childNode.isElement()) */
/*         { */
/*             QDomElement nodeElement = childNode.toElement(); */
/*             if(nodeElement.tagName() == */ 
/*                     LCXmlLayoutBuilderBase::mCommonTags.layout) */
/*             { */
/*                 QLayout* layout = nullptr; */

/*                 QSharedPointer<LIXmlLayoutBuilder> */ 
/*                     builder = */ 
/*                     _app.getLayoutBuilder( */
/*                             nodeElement.attribute( */
/*                                 LCXmlLayoutBuilderBase:: */
/*                                 mCommonLayoutsAttributes.layoutType)); */

/*                 if(!builder.isNull()) */
/*                 { */
/*                     layout = builder->build(nodeElement, _app); */
/*                 } */

/*                 if(layout) */
/*                 { */
/*                     _buildData.mpLayout-> */
/*                         addLayout(layout, _buildData.mRow, col); */
/*                 } */
/*                 else */
/*                 { */
/*                     _buildData.mpLayout-> */
/*                         addItem(new QSpacerItem(0,0), _buildData.mRow, col); */
/*                 } */
/*             } */
/*             else */
/*             { */
/*                 QWidget* widget = nullptr; */

/*                 QSharedPointer<LIXmlWidgetBuilder> wb = */ 
/*                     _app.getWidgetBuilder(nodeElement.tagName()); */

/*                 if(!wb.isNull()) */
/*                 { */
/*                     widget = wb->build(nodeElement, _app); */
/*                 } */

/*                 if(widget) */
/*                 { */
/*                     _buildData.mpLayout-> */
/*                         addWidget(widget, _buildData.mRow, col); */
/*                 } */
/*                 else */
/*                 { */
/*                     _buildData.mpLayout-> */
/*                         addItem(new QSpacerItem(0,0), _buildData.mRow, col); */
/*                 } */
/*             } */
/*             col++; */
/*         } */
/*         //-------------------------------- */
/*         childNode = childNode.nextSibling(); */
/*     } */
/*     _buildData.mRow++; */
/*     if(col > _buildData.mColumn) _buildData.mColumn = col; */
/* } */

/* //------------------------------------------------------------------------------ */
/* static void createCol( */
/*         const QDomElement &_element, */ 
/*         const LIApplication& _app, */ 
/*         SBuildData& _buildData) */
/* { */
/*     quint32 row = 0; */

/*     QDomNode childNode = _element.firstChild(); */

/*     if(childNode.isNull()) */
/*     { */
/*         _buildData.mpLayout-> */
/*             addItem(new QSpacerItem(0,0), row, _buildData.mColumn); */

/*         _buildData.mColumn++; */
/*         return; */
/*     } */

/*     while(!childNode.isNull()) */
/*     { */
/*         if(childNode.isElement()) */
/*         { */
/*             QDomElement nodeElement = childNode.toElement(); */
/*             if(nodeElement.tagName() == */ 
/*                     LCXmlLayoutBuilderBase::mCommonTags.layout) */
/*             { */
/*                 QLayout* layout = nullptr; */

/*                 QSharedPointer<LIXmlLayoutBuilder> */ 
/*                     builder = */
/*                     _app.getLayoutBuilder(nodeElement.attribute( */
/*                                 LCXmlLayoutBuilderBase:: */
/*                                 mCommonLayoutsAttributes.layoutType)); */

/*                 if(!builder.isNull()) */
/*                 { */
/*                     layout = builder->build(nodeElement, _app); */
/*                 } */

/*                 if(layout) */
/*                 { */
/*                     _buildData.mpLayout-> */
/*                         addLayout(layout, row, _buildData.mColumn); */
/*                 } */
/*                 else */
/*                 { */
/*                     _buildData.mpLayout-> */
/*                         addItem(new QSpacerItem(0,0), row, _buildData.mColumn); */
/*                 } */
/*             } */
/*             else */
/*             { */
/*                 QWidget* widget = nullptr; */

/*                 QSharedPointer<LIXmlWidgetBuilder> wb = */ 
/*                     _app.getWidgetBuilder(nodeElement.tagName()); */

/*                 if(!wb.isNull()) */
/*                 { */
/*                     widget = wb->build(nodeElement, _app); */
/*                 } */

/*                 if(widget) */
/*                 { */
/*                     _buildData.mpLayout-> */
/*                         addWidget(widget, row, _buildData.mColumn); */
/*                 } */
/*                 else */
/*                 { */
/*                     _buildData.mpLayout-> */
/*                         addItem(new QSpacerItem(0,0), row, _buildData.mColumn); */
/*                 } */
/*             } */
/*             row++; */
/*         } */
/*         //-------------------------------- */
/*         childNode = childNode.nextSibling(); */
/*     } */
/*     _buildData.mColumn++; */
/*     if(row > _buildData.mRow) _buildData.mRow = row; */
/* } */

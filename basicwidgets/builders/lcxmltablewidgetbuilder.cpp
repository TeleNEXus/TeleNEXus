#include "lcxmltablewidgetbuilder.h"
#include "lcbuilderscommon.h"
#include "LIApplication.h"

#include <QTableWidget>
#include <QDomElement>
#include <qnamespace.h>
#include <qtablewidget.h>
#include <QDebug>
#include <QEvent>
#include <QtWidgets>
//==============================================================================
static int __slObjectCounter = 0;

//------------------------------------------------------------------------------
static const struct
{
    QString width = "width";
    QString height = "height";
    QString bgColorHeader = "bgColorHeader";
    QString textColorHeader = "textColorHeader";
    QString fontHeader = "fontHeader";
    QString fontIdHeader = "fontIdHeader";
    QString gridColor = "gridColor";
} __attrNames;

//------------------------------------------------------------------------------
static const struct
{
    QString row = "row";
    QString column = "col";
} __elementNames;

//==============================================================================
LCXmlTableWidgetBuilder::LCXmlTableWidgetBuilder()
{

}

//------------------------------------------------------------------------------
LCXmlTableWidgetBuilder::~LCXmlTableWidgetBuilder()
{

}


//------------------------------------------------------------------------------
struct SBuildData
{
  /*
   * Фильтр обработки события нажатия клавиши Esc.
   */
  class CQEventFilter : public QObject
  {
  public:
    explicit CQEventFilter(QObject* _parent = nullptr) : QObject(_parent){}
    virtual bool eventFilter(QObject* _p_obj, QEvent* _p_event) override
    {
      if(_p_event->type() == QEvent::KeyRelease)
      {
        //Очиска фокуса видета при нажатии клавиши Escape.
        if( static_cast<QKeyEvent*>(_p_event)->key() == Qt::Key_Escape)
        {
          static_cast<QTableWidget*>(_p_obj)->clearFocus();
          static_cast<QTableWidget*>(_p_obj)->clearSelection();
          return true;
        }
      }
      return false;
    }
  };
  quint32 mRow = 0;
  quint32 mColumn = 0;
  QTableWidget* mpTable= nullptr;
  SBuildData() : mpTable(new QTableWidget)
  {
    mpTable->setSelectionMode(QTableWidget::SelectionMode::NoSelection);
    mpTable->installEventFilter(new CQEventFilter);
    mpTable->setEditTriggers(QTableWidget::EditTrigger::NoEditTriggers);
    mpTable->setObjectName(QString("XmlTableWidget_%1").arg(__slObjectCounter));
    __slObjectCounter++;
  }
};

//------------------------------------------------------------------------------
static QString getTableStyle(const QDomElement& _element, 
    const LIApplication& _app);

static QString getHeaderStyle(
    const QDomElement& _element, const LIApplication& _app);

static void createRow(
        const QDomElement &_element,
        const LIApplication& _app,
        SBuildData& _buildData);

static void createCol(
        const QDomElement &_element,
        const LIApplication& _app,
        SBuildData& _buildData);

//------------------------------------------------------------------------------
QWidget* LCXmlTableWidgetBuilder::buildLocal(const QDomElement& _element,
    const LIApplication& _app)
{
  SBuildData buildData;

  QDomNode childNode = _element.firstChild();
  for(QDomNode node = _element.firstChild();
      !node.isNull();
      node = node.nextSiblingElement())
  {
    if(childNode.isElement())
    {
      QDomElement element = node.toElement();
      if(element.tagName() == __elementNames.row)
      {
        createRow(element, _app, buildData);
      }
      else if(element.tagName() == __elementNames.column)
      {
        createCol(element, _app, buildData);
      }
    }
  }

  QString table_style = getTableStyle(_element, _app);

  table_style = QString("QTableWidget#%1 { %2 }").
    arg(buildData.mpTable->objectName()).
    arg(table_style);

  QString header_style = getHeaderStyle(_element, _app);
  header_style = QString(
      "QTableCornerButton { %1 }"
      "QHeaderView { %1 }").
    arg(header_style);
  qDebug() << "Table Widget Style Sheet = " << table_style + header_style;

  buildData.mpTable->setStyleSheet(table_style + header_style);

  /* buildData.mpTable->setStyleSheet( */
  /*     QString("QTableWidget#%1 { %2 }"). */
  /*     arg(buildData.mpTable->objectName()). */
  /*     arg(table_style)); */

  /* buildData.mpTable->setStyleSheet( */
  /*     "QTableWidget { " + style + " }" + */ 
  /*     "QTableCornerButton { " + style + " }" + */ 
  /*     "QHeaderView { " + style + " }" ); */

  LCBuildersCommon::initPosition(_element, *buildData.mpTable);
  return buildData.mpTable;
}

//------------------------------------------------------------------------------
static QString getTableStyle(const QDomElement& _element, const LIApplication& _app)
{
  QString style = LCBuildersCommon::getBaseStyleSheet(_element, _app);
  QString attr = _element.attribute(__attrNames.gridColor);
  if(!attr.isNull())
  {
    style += QString("gridline-color: %1;").arg(attr);
  }
  return style;
}

//------------------------------------------------------------------------------
static QString getHeaderStyle(const QDomElement& _element, const LIApplication& _app)
{
  QString style;

  QString attr =_element.attribute(__attrNames.bgColorHeader);
  if(!attr.isNull()) 
    style += QString("background: %1;").arg(attr);

  attr =_element.attribute(__attrNames.textColorHeader);
  if(!attr.isNull()) 
    style += QString("color: %1;").arg(attr);

  attr =_element.attribute(__attrNames.fontIdHeader);
  if(!attr.isNull())
  {
    QString font = _app.getFontStyle(attr);
    if(!font.isNull())
    {
      style += QString("font: %1;").arg(font);
    }
  }

  attr =_element.attribute(__attrNames.fontHeader);
  if(!attr.isNull()) 
    style += QString("font: %1;").arg(attr);

  attr =_element.attribute(__attrNames.gridColor);
  if(!attr.isNull()) 
    style += QString("gridline-color: %1;").arg(attr);
 
  return style;
}

//------------------------------------------------------------------------------
static void createRow(
        const QDomElement &_element, 
        const LIApplication& _app, 
        SBuildData& _buildData)
{
    quint32 col = 0;
    QDomNode childNode = _element.firstChild();

    _buildData.mpTable->setRowCount(++_buildData.mRow);

    QString row_name = _element.attribute(LCBuildersCommon::mAttributes.label);

    int curr_row = _buildData.mRow - 1;

    if(!row_name.isNull())
    {
        _buildData.mpTable->setVerticalHeaderItem(curr_row, 
                new QTableWidgetItem(row_name));
    }

    QString attr_height = _element.attribute(__attrNames.height);
    if(!attr_height.isNull())
    {
        bool flag = false;
        int height = attr_height.toInt(&flag);
        if(flag)
            _buildData.mpTable->setRowHeight(curr_row, height);
    }

    if(childNode.isNull())
    {
        return;
    }

    while(!childNode.isNull())
    {
        if(!childNode.isElement())
        {
            childNode = childNode.nextSibling();
            continue;
        }

        QDomElement nodeElement = childNode.toElement();

        QWidget* widget = nullptr;

        //Попытка получения построителя виджета.
        QSharedPointer<LIXmlWidgetBuilder> widget_builder = 
            _app.getWidgetBuilder(nodeElement.tagName());

        col++;
        if(col > _buildData.mColumn)  
        {
            _buildData.mpTable->setColumnCount(col);
            _buildData.mColumn = col;
        }

        if(!widget_builder.isNull())
        {
            widget = widget_builder->build(nodeElement, _app);
        }

        if(widget)
        {
            _buildData.mpTable->setCellWidget(
                    _buildData.mRow - 1, 
                    col - 1, 
                    widget);
        }

        //--------------------------------
        childNode = childNode.nextSibling();
    }
}

//------------------------------------------------------------------------------
static void createCol(
        const QDomElement &_element, 
        const LIApplication& _app, 
        SBuildData& _buildData)
{
    quint32 row = 0;
    QDomNode childNode = _element.firstChild();

    _buildData.mpTable->setColumnCount(++_buildData.mColumn);

    QString col_name = _element.attribute( LCBuildersCommon::mAttributes.label);

    if(!col_name.isNull())
    {
        _buildData.mpTable->setHorizontalHeaderItem(_buildData.mColumn -1, 
            new QTableWidgetItem(col_name));
    }

    QString attr_width = _element.attribute(__attrNames.width);
    if(!attr_width.isNull())
    {
        bool flag = false;
        int width = attr_width.toInt(&flag);
        if(flag)
            _buildData.mpTable->setColumnWidth(_buildData.mColumn -1, width);
    }

    if(childNode.isNull())
    {
        return;
    }

    while(!childNode.isNull())
    {
        if(!childNode.isElement())
        {
            childNode = childNode.nextSibling();
            continue;
        }

        QDomElement nodeElement = childNode.toElement();

        QWidget* widget = nullptr;
        //Попытка получения построителя виджета.
        QSharedPointer<LIXmlWidgetBuilder> widget_builder = 
            _app.getWidgetBuilder(nodeElement.tagName());

        //Добавление новой строки.
        row++;
        if(row > _buildData.mRow)  
        {
            _buildData.mpTable->setRowCount(row);
            _buildData.mRow = row;
        }

        if(!widget_builder.isNull())
        {
            widget = widget_builder->build(nodeElement, _app);
        }

        if(widget)
        {
            _buildData.mpTable->setCellWidget(
                    row - 1, 
                    _buildData.mColumn - 1, 
                    widget);
        }

        //--------------------------------
        childNode = childNode.nextSibling();
    }
}

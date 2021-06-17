/* 
 * TeleNEXus is a simple SCADA programm
 *
 * Copyright (C) 2020 Sergey S. Kuzmenko
 *
 * This file is part of TeleNEXus.
 *
 * TeleNEXus is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * TeleNEXus is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with TeleNEXus.  If not, see <https://www.gnu.org/licenses/>.
 */
#include "lcxmltablewidgetbuilder.h"
#include "LIApplication.h"

#include <QTableWidget>
#include <QDomElement>
#include <qnamespace.h>
#include <QTableWidget>
#include <QDebug>
#include <QEvent>
#include <QKeyEvent>

//------------------------------------------------------------------------------
static const struct
{
  QString label               = "label";
  QString width               = "width";
  QString height              = "height";

  QString styleTable          = "styleTable";
  QString styleHeader         = "styleHeader";
  QString styleCorner         = "styleCorner";

} __slAttributes;

//------------------------------------------------------------------------------
static const struct
{
  QString row = "row";
  QString column = "col";
} __slTags;

//==============================================================================
LCXmlTableWidgetBuilder::LCXmlTableWidgetBuilder()
{

}

//------------------------------------------------------------------------------
LCXmlTableWidgetBuilder::~LCXmlTableWidgetBuilder()
{

}


//------------------------------------------------------------------------------
struct SLocalData
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
  SLocalData() : mpTable(new QTableWidget)
  {
    mpTable->setSelectionMode(QTableWidget::SelectionMode::NoSelection);
    mpTable->installEventFilter(new CQEventFilter(mpTable));
    mpTable->setEditTriggers(QTableWidget::EditTrigger::NoEditTriggers);
  }
};

//------------------------------------------------------------------------------
static void createRow(
    const QDomElement &_element,
    const LIApplication& _app,
    SLocalData& _localData);

static void createCol(
    const QDomElement &_element,
    const LIApplication& _app,
    SLocalData& _localData);

//------------------------------------------------------------------------------
QWidget* LCXmlTableWidgetBuilder::buildLocal(
    const QDomElement& _element, const LIApplication& _app)
{
  SLocalData buildData;

  for(QDomNode node = _element.firstChild();
      !node.isNull();
      node = node.nextSibling())
  {
    if(!node.isElement()) continue;

    QDomElement nel = node.toElement();

    if(nel.tagName() == __slTags.row)
    {
      createRow(nel, _app, buildData);
    }
    else if(nel.tagName() == __slTags.column)
    {
      createCol(nel, _app, buildData);
    }
  }

  auto set_style_table = 
    [&buildData, &_element]()
    {
      QString attr_style = _element.attribute(__slAttributes.styleTable);
      if(attr_style.isNull()) return;

      /* QString style = _app.getWidgetStyle(attr_style); */
      /* if(style.isNull()) { style = attr_style;} */

      setWidgetStyle(
          QString("%1 %2 ").arg(buildData.mpTable->styleSheet()).arg(attr_style),
          buildData.mpTable, 
          QString("QTableWidget#%1 ")
          .arg(buildData.mpTable->objectName()));
    };

  auto set_style_header = 
    [&buildData, &_element]()
    {
      QString attr_style = _element.attribute(__slAttributes.styleHeader);
      if(attr_style.isNull()) return;

      setWidgetStyle(
          QString("%1 %2").arg(buildData.mpTable->styleSheet()).arg(attr_style),
          buildData.mpTable, 
          QString("QTableWidget#%1 > %2 ")
          .arg(buildData.mpTable->objectName())
          .arg(QStringLiteral("QHeaderView")));
    };

  auto set_style_corner = 
    [&buildData, &_element]()
    {
      QString attr_style = _element.attribute(__slAttributes.styleCorner);
      if(attr_style.isNull()) return;

      setWidgetStyle(
          QString("%1 %2").arg(buildData.mpTable->styleSheet()).arg(attr_style),
          buildData.mpTable, 
          QString("QTableWidget#%1 > %2 ")
          .arg(buildData.mpTable->objectName())
          .arg(QStringLiteral("QTableCornerButton")));
    };

  setWidgetName(      _element, buildData.mpTable);
  set_style_table();
  set_style_header();
  set_style_corner();
  setWidgetSize(      _element, buildData.mpTable);
  setWidgetFixedSize( _element, buildData.mpTable);
  setWidgetPosition(  _element, buildData.mpTable);

  return buildData.mpTable;
}

//------------------------------------------------------------------------------
static void createRow(
    const QDomElement &_element, 
    const LIApplication& _app, 
    SLocalData& _localData)
{
  quint32 col = 0;

  _localData.mpTable->setRowCount(++_localData.mRow);

  QString row_name = _element.attribute(__slAttributes.label);

  int curr_row = _localData.mRow - 1;

  if(!row_name.isNull())
  {
    _localData.mpTable->setVerticalHeaderItem(curr_row, 
        new QTableWidgetItem(row_name));
  }

  QString attr_height = _element.attribute(__slAttributes.height);
  if(!attr_height.isNull())
  {
    bool flag = false;
    int height = attr_height.toInt(&flag);
    if(flag)
      _localData.mpTable->setRowHeight(curr_row, height);
  }


  for(QDomNode node = _element.firstChildElement();
      !node.isNull();
      node = node.nextSiblingElement())
  {
    QDomElement nel = node.toElement();

    QWidget* widget = nullptr;

    //Попытка получения построителя виджета.
    QSharedPointer<LIXmlWidgetBuilder> widget_builder = 
      _app.getWidgetBuilder(nel.tagName());

    col++;
    if(col > _localData.mColumn)  
    {
      _localData.mpTable->setColumnCount(col);
      _localData.mColumn = col;
    }

    if(!widget_builder.isNull())
    {
      widget = widget_builder->build(nel, _app);
    }

    if(widget)
    {
      _localData.mpTable->setCellWidget(
          _localData.mRow - 1, 
          col - 1, 
          widget);
    }
  }
}

//------------------------------------------------------------------------------
static void createCol(
    const QDomElement &_element, 
    const LIApplication& _app, 
    SLocalData& _localData)
{
  quint32 row = 0;

  _localData.mpTable->setColumnCount(++_localData.mColumn);

  QString col_name = _element.attribute( __slAttributes.label);

  if(!col_name.isNull())
  {
    _localData.mpTable->setHorizontalHeaderItem(_localData.mColumn -1, 
        new QTableWidgetItem(col_name));
  }

  QString attr_width = _element.attribute(__slAttributes.width);
  if(!attr_width.isNull())
  {
    bool flag = false;
    int width = attr_width.toInt(&flag);
    if(flag)
      _localData.mpTable->setColumnWidth(_localData.mColumn -1, width);
  }

  for(QDomNode node = _element.firstChildElement();
      !node.isNull();
      node = node.nextSiblingElement())
  {
    QDomElement nel = node.toElement();

    QWidget* widget = nullptr;

    //Попытка получения построителя виджета.
    QSharedPointer<LIXmlWidgetBuilder> widget_builder = 
      _app.getWidgetBuilder(nel.tagName());

    //Добавление новой строки.
    row++;
    if(row > _localData.mRow)  
    {
      _localData.mpTable->setRowCount(row);
      _localData.mRow = row;
    }

    if(!widget_builder.isNull())
    {
      widget = widget_builder->build(nel, _app);
    }

    if(widget)
    {
      _localData.mpTable->setCellWidget(
          row - 1, 
          _localData.mColumn - 1, 
          widget);
    }
  }
}

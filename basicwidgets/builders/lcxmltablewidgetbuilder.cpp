#include "lcxmltablewidgetbuilder.h"
#include <QTableWidget>
#include <QDomElement>
#include <qnamespace.h>
#include <qtablewidget.h>
#include <QDebug>
#include "LIApplication.h"

//==============================================================================
LCXmlTableWidgetBuilder::LCXmlTableWidgetBuilder()
{

}

//------------------------------------------------------------------------------
LCXmlTableWidgetBuilder::~LCXmlTableWidgetBuilder()
{

}

//------------------------------------------------------------------------------
static const struct
{
    QString name = "name";
    QString width = "width";
    QString height = "height";
} __attrNames;

//------------------------------------------------------------------------------
static const struct
{
    QString row = "row";
    QString column = "col";
} __elementNames;
#include <QEvent>
#include <QtWidgets>
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
        mpTable->installEventFilter(new CQEventFilter);
        mpTable->setEditTriggers(QTableWidget::EditTrigger::NoEditTriggers);
    }
};

//------------------------------------------------------------------------------
static void createRow(
        const QDomElement &_element,
        const LIApplication& _app,
        SBuildData& _buildData);

static void createCol(
        const QDomElement &_element,
        const LIApplication& _app,
        SBuildData& _buildData);

//------------------------------------------------------------------------------
QWidget* LCXmlTableWidgetBuilder::build(const QDomElement& _element,
        const LIApplication& _app)
{
    SBuildData buildData;

    QDomNode childNode = _element.firstChild();

    while(!childNode.isNull())
    {
        if(childNode.isElement())
        {
            QDomElement element = childNode.toElement();
            if(element.tagName() == __elementNames.row)
            {
                createRow(element, _app, buildData);
            }
            else if(element.tagName() == __elementNames.column)
            {
                createCol(element, _app, buildData);

            }
        }
        //--------------------------------
        childNode = childNode.nextSibling();
    }
    return buildData.mpTable;

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

    QString row_name = _element.attribute(__attrNames.name);

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

    QString col_name = _element.attribute( __attrNames.name);

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

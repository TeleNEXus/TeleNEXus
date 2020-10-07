#include "lcxmltablewidgetbuilder.h"
#include <QTableWidget>
#include <QDomElement>
#include <qtablewidget.h>
#include <QDebug>

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
//    QString value       = "value";
} __attrNames;

//------------------------------------------------------------------------------
static const struct
{
    QString row = "row";
    QString column = "col";
} __elementNames;

//------------------------------------------------------------------------------
struct SBuildData
{
    quint32 mRow = 0;
    quint32 mColumn = 0;
    QTableWidget* mpTable= nullptr;
    SBuildData() : mpTable(new QTableWidget){}
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
    Q_UNUSED(_element);
    Q_UNUSED(_app);

    QTableWidget* table = new QTableWidget();
    qDebug() << "------------------------------------Create QTableWidget Start";
    table->setRowCount(5);
    table->setColumnCount(5);
    qDebug() << "Table widget row count     =" << table->rowCount();
    qDebug() << "Table widget column count  =" << table->columnCount();
    return table;
}

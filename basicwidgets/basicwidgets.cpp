#include "basicwidgets.h"

#include "lcxmlwidgetbuilder.h"
#include "lcxmltextlabelbuilder.h"
#include "lcxmlremlabelbuilder.h"
#include "lcxmlremlineeditbuilder.h"
#include "lcxmlremcomboboxbuilder.h"
#include "lcxmlremcombolabelbuilder.h"
#include "lcxmltablewidgetbuilder.h"
#include "lcxmltabwidgetbuilder.h"
//------------------------------------------------------------------------------
void* getWidgetBuilder()
{
    return new LCXmlWidgetBuilder;
}

//------------------------------------------------------------------------------
void* getTextLabelBuilder()
{
    return new LCXmlTextLabelBuilder;
}

//------------------------------------------------------------------------------
void* getRemLabelBuilder()
{
    return new LCXmlRemLabelBuilder;
}

//------------------------------------------------------------------------------
void*  getRemLineEditBuilder()
{
    return new LCXmlRemLineEditBuilder;
}

//------------------------------------------------------------------------------
void*  getRemComboBoxBuilder()
{
    return new LCXmlRemComboBoxBuilder;
}

//------------------------------------------------------------------------------
void*  getRemComboLabelBuilder()
{
    return new LCXmlRemComboLabelBuilder;
}

//------------------------------------------------------------------------------
void* getTableWidgetBuilder()
{
    return new LCXmlTableWidgetBuilder;
}

//------------------------------------------------------------------------------
void* getTabWidgetBuilder()
{
    return new LCXmlTabWidgetBuilder;
}

#include "lcxmlremwritebuttonbuilder.h"
//------------------------------------------------------------------------------
void* getRemWriteButtonBuilder()
{
    return new LCXmlRemWriteButtonBuilder;
}

//------------------------------------------------------------------------------
#include "lcxmlswitchwidgetslist.h"
void* getSwitchWidgetsListBuilder()
{
    return new LCXmlSwitchWidgetsListBuilder;
}



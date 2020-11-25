#include "basicwidgets.h"

#include "lcqremwritebutton.h"
#include "lcxmlwidgetbuilder.h"
#include "lcxmltextlabelbuilder.h"
#include "lcxmlremlabelbuilder.h"
#include "lcxmlremlineeditbuilder.h"
#include "lcxmlremcomboboxbuilder.h"
#include "lcxmlremcombolabelbuilder.h"
#include "lcxmltablewidgetbuilder.h"
#include "lcxmltabwidgetbuilder.h"
#include "lcxmlbuttonbuilder.h"
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

//------------------------------------------------------------------------------
void* getRemWriteButtonBuilder()
{
  return new LCXmlButtonBuilder(LCXmlButtonBuilder::EType::writeData);
}

//------------------------------------------------------------------------------
void* getControlWindowButtonBuilder()
{
  return new LCXmlButtonBuilder(LCXmlButtonBuilder::EType::controlWindows);
}

//------------------------------------------------------------------------------
#include "lcxmlswitchwidgetslistbuilder.h"
void* getSwitchWidgetsListBuilder()
{
    return new LCXmlSwitchWidgetsListBuilder;
}

//------------------------------------------------------------------------------
#include "lcxmlscrollareabuilder.h"
void* getScrollAreaBuilder()
{
    return new LCXmlScrollAreaBuilder;
}

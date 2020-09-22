#include "basicwidgets.h"

#include "lcxmlwidgetbuilder.h"
#include "lcxmltextlabelbuilder.h"
#include "lcxmlremlabelbuilder.h"
#include "lcxmlremlineeditbuilder.h"
#include "lcxmlremcomboboxbuilder.h"
#include "lcxmlremcombolabelbuilder.h"

//------------------------------------------------------------------------------
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

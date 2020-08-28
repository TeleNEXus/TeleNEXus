#include "basicwidgets.h"

#include "lcxmlwidgetbuilder.h"
#include "lcxmltextlabelbuilder.h"
#include "lcxmlremotedatalabelbuilder.h"
#include "lcxmlremotedatalineeditbuilder.h"

//----------------------------------------------------------------------------------------------------------------------
void* BASICWIDGETS_EXPORT getWidgetBuilder()
{
    return new LCXmlWidgetBuilder;
}

//----------------------------------------------------------------------------------------------------------------------
void* getTextLabelBuilder()
{
    return new LCXmlTextLabelBuilder;
}

//----------------------------------------------------------------------------------------------------------------------
void* BASICWIDGETS_EXPORT getRemoteDataLabelBuilder()
{
    return new LCXmlRemoteDataLabelBuilder;
}

//----------------------------------------------------------------------------------------------------------------------
void* BASICWIDGETS_EXPORT getRemoteDataLineEditBuilder()
{
    return new LCXmlRemoteDataLineEditBuilder;
}

#include "basicwidgets.h"

#include "lcxmlwidgetbuilder.h"
#include "lcxmltextlabelbuilder.h"
#include "lcxmlremotedatalabelbuilder.h"
#include "lcxmlremotedatalineeditbuilder.h"

//----------------------------------------------------------------------------------------------------------------------
void*  getWidgetBuilder()
{
    return new LCXmlWidgetBuilder;
}

//----------------------------------------------------------------------------------------------------------------------
void* getTextLabelBuilder()
{
    return new LCXmlTextLabelBuilder;
}

//----------------------------------------------------------------------------------------------------------------------
void* getRemoteDataLabelBuilder()
{
    return new LCXmlRemoteDataLabelBuilder;
}

//----------------------------------------------------------------------------------------------------------------------
void*  getRemoteDataLineEditBuilder()
{
    return new LCXmlRemoteDataLineEditBuilder;
}

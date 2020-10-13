#include "basiclayouts.h"
#include "lcxmlgridlayoutbuilder.h"
#include "lcxmlboxlayoutbuilder.h"


//------------------------------------------------------------------------------
void* BASICLAYOUTS_EXPORT getGridLayoutBuilder()
{
    return new LCXmlGridLayoutBuilder;
}

//----------------------------------------------------------------------------
void* BASICLAYOUTS_EXPORT getBoxLayoutBuilder()
{
    return new LCXmlBoxLayoutBuilder();
}


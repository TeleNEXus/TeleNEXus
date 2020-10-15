#include "basiclayouts.h"
#include "lcxmlgridlayoutbuilder.h"
#include "lcxmlboxlayoutbuilder.h"


//------------------------------------------------------------------------------
void* BASICLAYOUTS_EXPORT getGridLayoutBuilder()
{
    return new LCXmlGridLayoutBuilder;
}

//----------------------------------------------------------------------------
void* BASICLAYOUTS_EXPORT getVBoxLayoutBuilder()
{
    return new LCXmlBoxLayoutBuilder(LCXmlBoxLayoutBuilder::EOrientation::VERTICAL);
}

//----------------------------------------------------------------------------
void* BASICLAYOUTS_EXPORT getHBoxLayoutBuilder()
{
    return new LCXmlBoxLayoutBuilder(LCXmlBoxLayoutBuilder::EOrientation::HORIZONTAL);
}

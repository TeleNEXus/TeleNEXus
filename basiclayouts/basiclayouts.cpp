#include "basiclayouts.h"
#include "lcxmlgridlayoutbuilder.h"
#include "lcxmlboxlayoutbuilder.h"


//------------------------------------------------------------------------------
void* getGridLayoutBuilder()
{
    return new LCXmlGridLayoutBuilder;
}

//----------------------------------------------------------------------------
void* getVBoxLayoutBuilder()
{
    return new LCXmlBoxLayoutBuilder(
            LCXmlBoxLayoutBuilder::EOrientation::VERTICAL);
}

//----------------------------------------------------------------------------
void* getHBoxLayoutBuilder()
{
    return new LCXmlBoxLayoutBuilder(
            LCXmlBoxLayoutBuilder::EOrientation::HORIZONTAL);
}


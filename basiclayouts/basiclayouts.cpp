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
    return new LCXmlBoxLayoutBuilder(LCXmlBoxLayoutBuilder::EOrientation::VERTICAL);
}

//----------------------------------------------------------------------------
void* getHBoxLayoutBuilder()
{
    return new LCXmlBoxLayoutBuilder(LCXmlBoxLayoutBuilder::EOrientation::HORIZONTAL);
}

/* //------------------------------------------------------------------------------ */
/* void* BASICLAYOUTS_EXPORT getGridLayoutBuilder() */
/* { */
/*     return new LCXmlGridLayoutBuilder; */
/* } */

/* //---------------------------------------------------------------------------- */
/* void* BASICLAYOUTS_EXPORT getVBoxLayoutBuilder() */
/* { */
/*     return new LCXmlBoxLayoutBuilder(LCXmlBoxLayoutBuilder::EOrientation::VERTICAL); */
/* } */

/* //---------------------------------------------------------------------------- */
/* void* BASICLAYOUTS_EXPORT getHBoxLayoutBuilder() */
/* { */
/*     return new LCXmlBoxLayoutBuilder(LCXmlBoxLayoutBuilder::EOrientation::HORIZONTAL); */
/* } */

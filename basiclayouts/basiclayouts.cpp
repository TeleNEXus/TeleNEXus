#include "basiclayouts.h"
#include "lcxmlgridlayoutbuilder.h"
#include "lcxmlboxlayoutbuilder.h"


//------------------------------------------------------------------------------
void* BASICLAYOUTS_EXPORT getGridLayoutBuilder()
{
    return new LCXmlGridLayoutBuilder;
}

//----------------------------------------------------------------------------
void* BASICLAYOUTS_EXPORT getBoxLayoutBulder()
{
    return new LCXmlBoxLayoutBuilder();
}

/* //---------------------------------------------------------------------------- */
/* void* BASICLAYOUTS_EXPORT getBoxVLayoutBulder() */
/* { */
/*     return new LCXmlBoxLayoutBuilder(QBoxLayout::Direction::TopToBottom); */
/* } */

/* //---------------------------------------------------------------------------- */
/* void* BASICLAYOUTS_EXPORT getBoxHLayoutBulder() */
/* { */
/*     return new LCXmlBoxLayoutBuilder(QBoxLayout::Direction::LeftToRight); */
/* } */

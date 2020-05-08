#include "basiclayouts.h"
#include "lcxmlgridlayoutbuilder.h"
#include "lcxmlboxlayoutbuilder.h"


//----------------------------------------------------------------------------------------------------------------------
void* BASICLAYOUTS_EXPORT getGridLayoutBulder()
{
    return new LCXmlGridLayoutBuilder;
}

//----------------------------------------------------------------------------------------------------------------------
void* BASICLAYOUTS_EXPORT getBoxLayoutBulder()
{
    return new LCXmlBoxLayoutBuilder;
}

////----------------------------------------------------------------------------------------------------------------------
//void* BASICLAYOUTS_EXPORT getVBoxLayoutBulder()
//{
//    return new LCXmlBoxLayoutBuilder(QBoxLayout::Direction::TopToBottom);
//}

////----------------------------------------------------------------------------------------------------------------------
//void* BASICLAYOUTS_EXPORT getHBoxLayoutBulder()
//{
//    return new LCXmlBoxLayoutBuilder(QBoxLayout::Direction::LeftToRight);
//}

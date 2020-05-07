#include "basicwidgets.h"

#include "lcxmltextlabelbuilder.h"

void* getTextLabelBuilder()
{
    return new LCXmlTextLabelBuilder;
}

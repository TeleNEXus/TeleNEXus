#include "basicwidgets.h"

#include "lcxmltextlabelbuilder.h"

void* getTextLabelBuilder()
{
    return new LCXmlTextLabelBuilder;
}

void* BASICWIDGETS_EXPORT getRemoteDataLabelBuilder()
{

}

void* BASICWIDGETS_EXPORT getRemoteDataLineEditBuilder()
{

}

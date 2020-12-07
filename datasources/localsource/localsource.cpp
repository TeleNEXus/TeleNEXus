#include "localsource.h"

#include "lcxmllocalsourcebuilder.h"

void* getLocalSourceBuilder()
{
    return new LCXmlLocalSourceBuilder;
}

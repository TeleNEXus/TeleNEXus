#include "mbsource.h"

#include "lcxmlmodbussourcebuilder.h"

void* getModbusSourceBuilder()
{
    return new LCXmlModbusSourceBuilder;
}

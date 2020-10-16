#include "common.h"

//==============================================================================
CCommonAttributes& CCommonAttributes::instance()
{
    static CCommonAttributes instance;
    return instance;
}

//------------------------------------------------------------------------------
Qt::AlignmentFlag 
CCommonAttributes::SAligns::toFlags(const QString& _attributes)
{
    quint16 flags = 0;

    return static_cast<Qt::AlignmentFlag>(flags);
}

#include "common.h"
#include <qnamespace.h>

//==============================================================================
const CCommonAttributes::SAligns CCommonAttributes::mAligns;

CCommonAttributes::CCommonAttributes()
{
}

//------------------------------------------------------------------------------
Qt::AlignmentFlag 
CCommonAttributes::SAligns::toFlags(const QString& _attributes) const
{
    quint16 flags = 0;

    if(_attributes.isNull())
    {
        return Qt::AlignLeft;
    }

    if(_attributes.contains(vals.Left))
    {
        flags |= Qt::AlignLeft;
    }

    if(_attributes.contains(vals.Right))
    {
        flags |= Qt::AlignRight;
    }

    if(_attributes.contains(vals.Center))
    {
        flags |= Qt::AlignCenter;
    }
    
    if(_attributes.contains(vals.HCenter))
    {
        flags |= Qt::AlignHCenter;
    }

    if(_attributes.contains(vals.Top))
    {
        flags |= Qt::AlignTop;
    }

    if(_attributes.contains(vals.Bottom))
    {
        flags |= Qt::AlignBottom;
    }

    if(_attributes.contains(vals.VCenter))
    {
        flags |= Qt::AlignVCenter;
    }

    if(flags == 0) return Qt::AlignLeft;

    return static_cast<Qt::AlignmentFlag>(flags);
}

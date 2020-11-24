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

    QString alines = _attributes.toLower();

    if(alines.contains(vals.Left))
    {
        flags |= Qt::AlignLeft;
    }

    if(alines.contains(vals.Right))
    {
        flags |= Qt::AlignRight;
    }

    if((alines.contains(vals.Center)) && 
        (!alines.contains(vals.HCenter)) &&
        (!alines.contains(vals.VCenter)))
    {
        flags |= Qt::AlignCenter;
    }
    
    if(alines.contains(vals.HCenter))
    {
        flags |= Qt::AlignHCenter;
    }

    if(alines.contains(vals.Top))
    {
        flags |= Qt::AlignTop;
    }

    if(alines.contains(vals.Bottom))
    {
        flags |= Qt::AlignBottom;
    }

    if(alines.contains(vals.VCenter))
    {
        flags |= Qt::AlignVCenter;
    }

    if(flags == 0) return Qt::AlignLeft;

    return static_cast<Qt::AlignmentFlag>(flags);
}

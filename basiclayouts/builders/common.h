#ifndef COMMON_H_
#define COMMON_H_
/*
 * Общие данные и функции для всех построителей.
 */
#include <QString>
#include <qnamespace.h>

class CCommonAttributes
{
public:
    struct SAligns{
        const QString attrName = "align";
        const struct
        {
            QString Left    = "Left";
            QString Right   = "Right";
            QString Center  = "Center";
            QString HCenter = "HCenter";
            QString Top     = "Top";
            QString Bottom  = "Bottom";
            QString VCenter = "VCenter";
        }vals;
        Qt::AlignmentFlag toFlags(const QString& _attributes);
        SAligns(){}
    };
    SAligns mAligns;
private:
    CCommonAttributes();
public:
    static CCommonAttributes& instance();
};
#endif

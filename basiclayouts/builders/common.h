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
            QString Left    = "left";
            QString Right   = "right";
            QString Center  = "center";
            QString HCenter = "hcenter";
            QString Top     = "top";
            QString Bottom  = "bottom";
            QString VCenter = "vcenter";
        }vals;
        Qt::AlignmentFlag toFlags(const QString& _attributes) const;
        SAligns(){}
    };
    static const SAligns mAligns;
private:
    CCommonAttributes();
};
#endif

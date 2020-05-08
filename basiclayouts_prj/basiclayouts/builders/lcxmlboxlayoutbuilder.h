#ifndef LCXMLBOXLAYOUTBUILDER_H
#define LCXMLBOXLAYOUTBUILDER_H

#include "lcxmllayoutbuilderbase.h"
#include <QBoxLayout>

class LCXmlBoxLayoutBuilder : public LCXmlLayoutBuilderBase
{
private:
    bool mFlagDirModeDef;
    QBoxLayout::Direction mDirectionDef;
public:
    LCXmlBoxLayoutBuilder();
    LCXmlBoxLayoutBuilder(QBoxLayout::Direction _direction);
    virtual ~LCXmlBoxLayoutBuilder();
    virtual QLayout* build(const QDomElement& _element, const LIApplication& _app) override;

};
#endif // LCXMLBOXLAYOUTBUILDER_H

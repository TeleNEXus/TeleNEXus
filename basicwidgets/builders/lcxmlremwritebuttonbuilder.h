#include "LIXmlWidgetBuilder.h"
#include "builderscommon.h"

class LCXmlRemWriteButtonBuilder : public LIXmlWidgetBuilder
{
private:
    struct SAttributes
    {
        QString label = "label";
    };
    static const SAttributes mAttributes;
    
public:
    LCXmlRemWriteButtonBuilder();
    ~LCXmlRemWriteButtonBuilder();
    virtual QWidget* build(
            const QDomElement& _element, const LIApplication& _app) override;

};

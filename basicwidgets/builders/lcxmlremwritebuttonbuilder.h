#include <QString>
#include "LIXmlWidgetBuilder.h"
class LCXmlRemWriteButtonBuilder : public LIXmlWidgetBuilder
{
public:

    struct SAttributes
    {
        QString dataName    = "dataName";
        QString data        = "data";
        QString format      = "format";
    };

    /* struct STags */
    /* { */
    /* }; */
    
    SAttributes mAttributes;
public:
    LCXmlRemWriteButtonBuilder();
    ~LCXmlRemWriteButtonBuilder();
    virtual QWidget* build(
            const QDomElement& _element, const LIApplication& _app) override;

};

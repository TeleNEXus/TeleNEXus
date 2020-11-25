#ifndef LCXMLTEXTLABELBUILDER_H
#define LCXMLTEXTLABELBUILDER_H

#include "lcxmlbuilderbase.h"

class LCXmlTextLabelBuilder : public LCXmlBuilderBase
{
public:
  LCXmlTextLabelBuilder();
  virtual ~LCXmlTextLabelBuilder();
protected:
  virtual QWidget* buildLocal( 
      const QDomElement& _element, 
      const LIApplication& _app) override;
};

#endif // LCXMLTEXTLABELBUILDER_H

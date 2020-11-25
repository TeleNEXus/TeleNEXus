#ifndef LCXMLTABBUILDER_H_
#define LCXMLTABBUILDER_H_

#include "lcxmlbuilderbase.h"

class LCXmlTabWidgetBuilder : public LCXmlBuilderBase
{
public:
  LCXmlTabWidgetBuilder();
  virtual ~LCXmlTabWidgetBuilder();

protected: 
  virtual QWidget* buildLocal( 
      const QDomElement& _element, 
      const LIApplication& _app) override;
};
#endif /* LCXMLTABBUILDER_H_ */

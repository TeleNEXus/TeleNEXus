#ifndef LCXMLSCROLLAREABUILDER_H_ 
#define LCXMLSCROLLAREABUILDER_H_

#include "lcxmlbuilderbase.h"

class LCXmlScrollAreaBuilder: public LCXmlBuilderBase 
{
public:
  LCXmlScrollAreaBuilder();
  virtual ~LCXmlScrollAreaBuilder();
protected:
  virtual QWidget* buildLocal( const QDomElement& _element, 
      const LIApplication& _app) override;
};

#endif /* LCXMLSCROLLAREABUILDER_H_ */


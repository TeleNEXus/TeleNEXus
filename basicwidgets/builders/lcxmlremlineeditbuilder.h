#ifndef LCXMLREMLINEEDITBUILDER_H
#define LCXMLREMLINEEDITBUILDER_H

#include "lcxmlbuilderbase.h"

class LCXmlRemLineEditBuilder : public LCXmlBuilderBase 
{

public:
  LCXmlRemLineEditBuilder();
  virtual ~LCXmlRemLineEditBuilder();

protected:
  virtual QWidget* buildLocal( const QDomElement& _element, 
      const LIApplication& _app) override;

};


#endif // LCXMLREMLINEEDITBUILDER_H

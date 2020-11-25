#ifndef LCXMLREMLABELBUILDER_H
#define LCXMLREMLABELBUILDER_H

#include "lcxmlbuilderbase.h"


class LCXmlRemLabelBuilder : public LCXmlBuilderBase
{
public:
  LCXmlRemLabelBuilder();
  virtual ~LCXmlRemLabelBuilder();

protected:
  virtual QWidget* buildLocal( const QDomElement& _element, 
      const LIApplication& _app) override final;
};

#endif // LCXMLREMLABELBUILDER_H

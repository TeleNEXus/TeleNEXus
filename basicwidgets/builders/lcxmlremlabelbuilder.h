#ifndef LCXMLREMLABELBUILDER_H
#define LCXMLREMLABELBUILDER_H

#include "LIXmlWidgetBuilder.h"


class LCXmlRemLabelBuilder : public LIXmlWidgetBuilder
{
public:
  LCXmlRemLabelBuilder();
  virtual ~LCXmlRemLabelBuilder();
  virtual QWidget* build( const QDomElement& _element, 
      const LIApplication& _app) override;
};

#endif // LCXMLREMLABELBUILDER_H

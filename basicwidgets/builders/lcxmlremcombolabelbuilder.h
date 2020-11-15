#ifndef LCXMLREMCOMBOLABELBUILDER_H
#define LCXMLREMCOMBOLABELBUILDER_H

#include "LIXmlWidgetBuilder.h"


class LCXmlRemComboLabelBuilder: public LIXmlWidgetBuilder
{
public:
  LCXmlRemComboLabelBuilder();
  virtual ~LCXmlRemComboLabelBuilder();
  virtual QWidget* build( const QDomElement& _element, 
      const LIApplication& _app) override;
};

#endif // LCXMLREMCOMBOLABELBUILDER_H

#ifndef LCXMLREMCOMBOLABELBUILDER_H
#define LCXMLREMCOMBOLABELBUILDER_H

#include "lcxmlbuilderbase.h"

class LCXmlRemComboLabelBuilder: public LCXmlBuilderBase 
{
public:
  LCXmlRemComboLabelBuilder();
  virtual ~LCXmlRemComboLabelBuilder();

protected:
  virtual QWidget* buildLocal( const QDomElement& _element, 
      const LIApplication& _app) override final;
};

#endif // LCXMLREMCOMBOLABELBUILDER_H

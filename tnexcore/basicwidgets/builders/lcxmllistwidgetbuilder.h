#ifndef LCXMLLISTWIDGETBUILDER_H_
#define LCXMLLISTWIDGETBUILDER_H_

#include "lcxmlbuilderbase.h"

class LCXmlListWidgetBuilder : public LCXmlBuilderBase
{
public:
  LCXmlListWidgetBuilder();
  virtual ~LCXmlListWidgetBuilder();

protected:
  virtual QWidget* buildLocal(
      const QDomElement& _element, const LIApplication& _app) override final;
};

#endif /*LCXMLLISTWIDGETBUILDER_H_*/

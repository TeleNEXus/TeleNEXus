#ifndef LCXMLBUILDERBASE_H_
#define LCXMLBUILDERBASE_H_

#include "LIXmlWidgetBuilder.h"

class LCXmlBuilderBase : public LIXmlWidgetBuilder
{
public:

  LCXmlBuilderBase();
  virtual ~LCXmlBuilderBase();

  virtual QWidget* build( 
      const QDomElement&      _element, 
      const LIApplication&    _app) override final;

protected:

  virtual QWidget* buildLocal(
      const QDomElement&      _element, 
      const LIApplication&    _app) = 0;
};

#endif

#ifndef LCXMLBUILDERBASE_H_
#define LCXMLBUILDERBASE_H_

#include "LIXmlWidgetBuilder.h"
#include <QPoint>
#include <QSharedPointer>
#include <qglobal.h>

class LCXmlBuilderBase : public LIXmlWidgetBuilder
{
protected:
  struct SBuildData
  {
    QPoint pos;
    const QDomElement& element;
    const LIApplication& application;
  };
public:

  LCXmlBuilderBase();
  virtual ~LCXmlBuilderBase();

  virtual QWidget* build( 
      const QDomElement&      _element, 
      const LIApplication&    _app) override final;

protected:

  virtual QWidget* buildLocal(QSharedPointer<SBuildData> _buildData) = 0;
};

#endif

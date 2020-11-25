#ifndef LCXMLBUILDERBASE_H_
#define LCXMLBUILDERBASE_H_

#include "LIXmlWidgetBuilder.h"
#include <QPoint>
#include <QSharedPointer>
#include <qglobal.h>

class LCXmlBuilderBase : public LIXmlWidgetBuilder
{
protected:
  struct SBaseData
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

  virtual QWidget* buildLocal(
      const QDomElement&      _element, 
      const LIApplication&    _app) = 0;

  virtual QWidget* buildLocal1(QSharedPointer<SBaseData> _buildData)
  {Q_UNUSED(_buildData); return nullptr;}
};

#endif

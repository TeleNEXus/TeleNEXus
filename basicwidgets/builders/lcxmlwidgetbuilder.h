#ifndef LCXMLWIDGETBUILDER_H
#define LCXMLWIDGETBUILDER_H

#include "lcxmlbuilderbase.h"

class LCXmlWidgetBuilder : public LCXmlBuilderBase
{
public:
  LCXmlWidgetBuilder();
  virtual ~LCXmlWidgetBuilder();

protected:

  virtual QWidget* buildLocal(
      QSharedPointer<SBuildData> _buildData) override final;
};

#endif // LCXMLWIDGETBUILDER_H

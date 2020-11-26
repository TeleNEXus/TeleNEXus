#ifndef LCXMLTABBUILDER_H_
#define LCXMLTABBUILDER_H_

#include "lcxmlbuilderbase.h"

class LCXmlTabWidgetBuilder : public LCXmlBuilderBase
{
public:
  LCXmlTabWidgetBuilder();
  virtual ~LCXmlTabWidgetBuilder();

protected: 
  virtual QWidget* buildLocal(
      QSharedPointer<SBuildData> _buildData) override final;
};
#endif /* LCXMLTABBUILDER_H_ */

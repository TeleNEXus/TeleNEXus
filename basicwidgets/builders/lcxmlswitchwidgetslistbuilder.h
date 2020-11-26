#ifndef LCXMLSWITCHWIDGETSLISTBUILDER_H_
#define LCXMLSWITCHWIDGETSLISTBUILDER_H_

#include "lcxmlbuilderbase.h"

class LCXmlSwitchWidgetsListBuilder : public LCXmlBuilderBase 
{
public:
  LCXmlSwitchWidgetsListBuilder();
  virtual ~LCXmlSwitchWidgetsListBuilder();

protected:
  virtual QWidget* buildLocal( 
      QSharedPointer<SBuildData> _buildData) override final;
};
#endif /* LCXMLSWITCHWIDGETSLISTBUILDER_H_ */

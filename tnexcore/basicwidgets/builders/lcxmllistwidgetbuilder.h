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
      QSharedPointer<SBuildData> _buildData) override final;
};

#endif /*LCXMLLISTWIDGETBUILDER_H_*/

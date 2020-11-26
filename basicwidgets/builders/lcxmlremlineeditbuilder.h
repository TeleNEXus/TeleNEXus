#ifndef LCXMLREMLINEEDITBUILDER_H
#define LCXMLREMLINEEDITBUILDER_H

#include "lcxmlbuilderbase.h"

class LCXmlRemLineEditBuilder : public LCXmlBuilderBase 
{

public:
  LCXmlRemLineEditBuilder();
  virtual ~LCXmlRemLineEditBuilder();

protected:
  virtual QWidget* buildLocal(
      QSharedPointer<SBuildData> _buildData) override final;

};


#endif // LCXMLREMLINEEDITBUILDER_H

#ifndef LCXMLSCROLLAREABUILDER_H_ 
#define LCXMLSCROLLAREABUILDER_H_

#include "lcxmlbuilderbase.h"

class LCXmlScrollAreaBuilder: public LCXmlBuilderBase 
{
public:
  LCXmlScrollAreaBuilder();
  virtual ~LCXmlScrollAreaBuilder();
protected:
  virtual QWidget* buildLocal( 
      QSharedPointer<SBuildData> _buildData) override final;
};

#endif /* LCXMLSCROLLAREABUILDER_H_ */


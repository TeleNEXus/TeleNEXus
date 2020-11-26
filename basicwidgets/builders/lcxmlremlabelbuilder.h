#ifndef LCXMLREMLABELBUILDER_H
#define LCXMLREMLABELBUILDER_H

#include "lcxmlbuilderbase.h"


class LCXmlRemLabelBuilder : public LCXmlBuilderBase
{
public:
  LCXmlRemLabelBuilder();
  virtual ~LCXmlRemLabelBuilder();

protected:
  virtual QWidget* buildLocal( 
      QSharedPointer<SBuildData> _buildData) override final;
};

#endif // LCXMLREMLABELBUILDER_H

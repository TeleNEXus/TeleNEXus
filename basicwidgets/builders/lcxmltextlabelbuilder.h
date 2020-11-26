#ifndef LCXMLTEXTLABELBUILDER_H
#define LCXMLTEXTLABELBUILDER_H

#include "lcxmlbuilderbase.h"

class LCXmlTextLabelBuilder : public LCXmlBuilderBase
{
public:
  LCXmlTextLabelBuilder();
  virtual ~LCXmlTextLabelBuilder();
protected:
  virtual QWidget* buildLocal(
      QSharedPointer<SBuildData> _buildData) override final;
};

#endif // LCXMLTEXTLABELBUILDER_H

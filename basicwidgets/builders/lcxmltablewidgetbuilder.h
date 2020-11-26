#ifndef LCQTABLEWIDGETBUILDER_H
#define LCQTABLEWIDGETBUILDER_H

#include "lcxmlbuilderbase.h"

class LCXmlTableWidgetBuilder : public LCXmlBuilderBase 
{
public:
    LCXmlTableWidgetBuilder();
    virtual ~LCXmlTableWidgetBuilder();

protected:
  virtual QWidget* buildLocal(
    QSharedPointer<SBuildData> _buildData) override final;
};

#endif /* LCQTABLEWIDGETBUILDER_H */

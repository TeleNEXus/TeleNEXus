#include "lcxmllocalsourcebuilder.h"
#include "lclocalsource.h"
#include "LIApplication.h"

#include <QFile>
#include <QDomElement>
#include <QDebug>


//==============================================================================
LCXmlLocalSourceBuilder::LCXmlLocalSourceBuilder()
{

}

//------------------------------------------------------------------------------
LQDataSources LCXmlLocalSourceBuilder::build(
    const QDomElement &_element, 
    const LIApplication& _app)
{
  Q_UNUSED(_element);
  Q_UNUSED(_app);
  LQDataSources ret_sources;
  return ret_sources; 
}


#include "lcqjsapplicationinterface.h"
#include "lcxmlmain.h"
#include "LIApplication.h"
#include <QDebug>

//==============================================================================
LCQJSApplicationInterface::LCQJSApplicationInterface(const LIApplication& _app):
  edApplication(_app)
{
}

//------------------------------------------------------------------------------
LCQJSApplicationInterface& LCQJSApplicationInterface::instance()
{
  static LCQJSApplicationInterface inst(LCXmlMain::getApplicationInterface());
  return inst;
}

//------------------------------------------------------------------------------
void LCQJSApplicationInterface::print(const QString& _str)
{
  qDebug() << _str;
}

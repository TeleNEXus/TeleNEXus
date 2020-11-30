#include "lcqjsapplicationinterface.h"
#include "lcxmlmain.h"
#include "LIApplication.h"
#include <QDebug>

//==============================================================================
LCQJSApplicationInterface::LCQJSApplicationInterface()
{
}

LCQJSApplicationInterface::~LCQJSApplicationInterface()
{
  qDebug() << "LCQJSApplicationInterface destructor";
}
//------------------------------------------------------------------------------
void LCQJSApplicationInterface::printapp(const QString& _str)
{
  qDebug() << _str;
}

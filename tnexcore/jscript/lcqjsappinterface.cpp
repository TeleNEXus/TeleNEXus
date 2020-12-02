#include "lcqjsappinterface.h"
#include "lcxmlmain.h"
#include "LIApplication.h"
#include <QDebug>

//==============================================================================
LCQJSAppInterface::LCQJSAppInterface() : 
  edApplication(LCXmlMain::getApplicationInterface())
{
}

//------------------------------------------------------------------------------
LCQJSAppInterface::~LCQJSAppInterface()
{
}

//------------------------------------------------------------------------------
void LCQJSAppInterface::print(QString _str)
{
  qDebug() << _str;
  qDebug() << "Project path = " << edApplication.getProjectPath();
}

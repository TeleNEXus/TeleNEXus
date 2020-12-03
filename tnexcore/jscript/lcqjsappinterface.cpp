#include "lcqjsappinterface.h"
#include "lcqjsappservice.h"
#include "LIApplication.h"
#include <QDebug>
#include <QSharedPointer>

//==============================================================================
struct SPrivateData
{
  QSharedPointer<LCQJSAppService> psAppService;
  SPrivateData() :
    psAppService(LCQJSAppService::getService()){}
};

//------------------------------------------------------------------------------
#define mpPrivateData (static_cast<SPrivateData*>(mpData))

//==============================================================================
LCQJSAppInterface::LCQJSAppInterface() :
  mpData(new SPrivateData)
{
}

//------------------------------------------------------------------------------
LCQJSAppInterface::~LCQJSAppInterface()
{
  delete mpPrivateData;
}

//------------------------------------------------------------------------------
void LCQJSAppInterface::print(QString _str)
{
  qDebug() << _str;
}

//------------------------------------------------------------------------------
QString LCQJSAppInterface::getProjectPath()
{
  return mpPrivateData->psAppService->getProjectPath();
}

//------------------------------------------------------------------------------
QString LCQJSAppInterface::readSourceData(const QString& _dataId)
{
  return mpPrivateData->psAppService->readSourceData(_dataId); 
}

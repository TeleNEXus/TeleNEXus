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
QVariantList LCQJSAppInterface::readSourceData(
    const QString& _sourceId, const QString& _dataId)
{
  QByteArray data_array = 
    mpPrivateData->psAppService->readSourceData(_sourceId, _dataId); 
  QVariantList ret;
  for(int i = 0; i < data_array.size(); i++)
  {
    ret << QVariant(static_cast<unsigned char>(data_array.at(i)));
  }
  return ret; 
}

//------------------------------------------------------------------------------
QVariantList LCQJSAppInterface::testGetByteArray()
{
  /* int data = 0x01020304; */
  int data = 0x01020304;
  QVariantList vl;
  /* QByteArray arr(reinterpret_cast<const char*>(&i), sizeof(int)); */
  for(int i = 0; i < (int)sizeof(int); i++)
  {
    vl << QVariant((reinterpret_cast<char*>(&data))[i]);
    /* vl << QVariant(i); */
  }
  return vl;
  
  /* _retString = "SByteArray LCQJSAppInterface::testGetByteArray(QString& _retString)"; */
  /* return (reinterpret_cast<const char*>(&i), sizeof(int)); */
}

#include "lclocalsource.h"
#include "lcqlocaldatareader.h"
#include "lcqlocaldatawriter.h"
#include "lcqlocalsourcehiden.h"

#include <QThread>

//==============================================================================SLocalData
struct SLocalData
{
  QThread* mpThread;
  QSharedPointer<LCQLocalSourceHiden> mspLocalSourceHiden;
  SLocalData() :
    mpThread(new QThread)
  {
    mspLocalSourceHiden = LCQLocalSourceHiden::create();
    mspLocalSourceHiden.data()->moveToThread(mpThread);
    mpThread->start();
  }

  ~SLocalData()
  {
    mpThread->quit();
    mpThread->wait(1000);
    mpThread->deleteLater();
  }
};

//------------------------------------------------------------------------------
#define mpLocalData (static_cast<SLocalData*>(mpData))

//==============================================================================LCLocalDataSource
LCLocalDataSource::LCLocalDataSource()
{
  mpData = new SLocalData();
}

//------------------------------------------------------------------------------
LCLocalDataSource::~LCLocalDataSource()
{
  delete mpLocalData;
}

//------------------------------------------------------------------------------
QSharedPointer<LCLocalDataSource> LCLocalDataSource::create()
{
  return QSharedPointer<LCLocalDataSource>(new LCLocalDataSource());
}

//------------------------------------------------------------------------------
void LCLocalDataSource::addByteItem(const QString& _dataName, const QByteArray& _data)
{
  mpLocalData->mspLocalSourceHiden->addItem(_dataName, _data);
}

//------------------------------------------------------------------------------
void LCLocalDataSource::addBitItem(const QString& _dataName, const QBitArray& _data)
{
  mpLocalData->mspLocalSourceHiden->addItem(_dataName, _data);
}

QSharedPointer<LIRemoteDataReader> LCLocalDataSource::createReader(
    const QString& _dataName,
    QWeakPointer<LIRemoteDataReadListener> _readListener)
{
  return mpLocalData->mspLocalSourceHiden->createReader(
      _dataName, 
      _readListener);
}

QSharedPointer<LIRemoteDataWriter> LCLocalDataSource::createWriter(
    const QString& _dataName,
    LTWriteListener _writeListener)
    /* QWeakPointer<LIRemoteDataWriteListener> _writeListener) */
{
  return mpLocalData->mspLocalSourceHiden->createWriter(
      _dataName, 
      _writeListener);
}




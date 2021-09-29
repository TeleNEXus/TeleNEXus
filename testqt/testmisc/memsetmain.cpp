#include <QCoreApplication>
#include <QDebug>
#include "lcmemoryset.h"


//==============================================================================CTestData
class CTestData : public LCMemorySetAccess::CIData
{
private:
  int mAddress;
  QByteArray mData;
public:
  CTestData() = delete;
  CTestData(int _addr, int _size) :
    mAddress(_addr),
    mData(QByteArray(_size, 0)){}

  virtual int getAddress() const override {return mAddress;}
  virtual int getSize() const override { return mData.size();}
  virtual void setData(const QByteArray& _data, int) override 
  {
    qDebug() << "mData.size() = "<< _data.size();
    if(_data.size() != mData.size()) 
    {
      qDebug() << "CTestData: Wrong set data.";
      return;
    }
    qDebug() << "Replace";
    mData.replace(0, mData.size(), _data);
  }
};

//==============================================================================
static void addItems(LCMemorySetAccess& _memorySetAccess);

//==============================================================================
int main(int argc, char** argv)
{
  Q_UNUSED(argc);
  Q_UNUSED(argv);
  /* QCoreApplication app(argc, argv); */
  /* return app.exec(); */


  qDebug() << "===========================================";
  qDebug() << "Test Memory set";
  qDebug() << "===========================================";

  auto reader = 
    [](int _addr, QByteArray& _data)
    {
      qDebug() << QString("Read data addr = %1; size = %2").arg(_addr).arg(_data.size());
      return 0;
    };

  LCMemorySetAccess memoryaccess(reader);
  addItems(memoryaccess);
  memoryaccess.update();
  return 0;
}

//==============================================================================
static void addItems(LCMemorySetAccess& _memorySetAccess)
{

  _memorySetAccess.insert(QSharedPointer<CTestData>(new CTestData(19, 2)));
  _memorySetAccess.insert(QSharedPointer<CTestData>(new CTestData( 3, 1)));
  _memorySetAccess.insert(QSharedPointer<CTestData>(new CTestData(13, 2)));
  _memorySetAccess.insert(QSharedPointer<CTestData>(new CTestData( 1, 1)));
  _memorySetAccess.insert(QSharedPointer<CTestData>(new CTestData(10, 4)));
  _memorySetAccess.insert(QSharedPointer<CTestData>(new CTestData( 0, 1)));
  _memorySetAccess.insert(QSharedPointer<CTestData>(new CTestData(15, 2)));
  _memorySetAccess.insert(QSharedPointer<CTestData>(new CTestData(17, 2)));
  _memorySetAccess.insert(QSharedPointer<CTestData>(new CTestData( 4, 1)));
  _memorySetAccess.insert(QSharedPointer<CTestData>(new CTestData( 2, 1)));
  _memorySetAccess.insert(QSharedPointer<CTestData>(new CTestData(17, 8)));
}


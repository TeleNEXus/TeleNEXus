#include <QCoreApplication>
#include <QDebug>
#include "lcmemoryset.h"


QByteArray __sReadDataArray(50, 0);


//==============================================================================CTestData
class CTestData : public LCMemoryReadSet::CIData
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
  virtual void setData(const QByteArray& _data, int _status) override
  {
    if((_status < 0) || (mData.size() != _data.size()))
    {
      return;
    }
    mData.replace(0, mData.size(), _data);
  }
  virtual QByteArray getData()const override { return mData; }
};

static QMap<QString, QSharedPointer<CTestData>> __slDataMap;

//==============================================================================
static void initDataMap();
static void addItems(LCMemoryReadSet& _memorySetAccess);
static void printData();

//==============================================================================
int main(int argc, char** argv)
{
  Q_UNUSED(argc);
  Q_UNUSED(argv);
  /* QCoreApplication app(argc, argv); */
  /* return app.exec(); */

  []()
  {
    for(int i = 0; i < __sReadDataArray.size(); i++)
    {
      /* __sReadDataArray[i] = i*1000; */
      __sReadDataArray[i] = i;
    }
  }();


  qDebug() << "===========================================";
  qDebug() << "Test Memory set";
  qDebug() << "===========================================";

  auto reader =
    [](int _addr, QByteArray& _data)
    {
      qDebug() << QString("Read data addr = %1; size = %2").arg(_addr).arg(_data.size());
      if(_data.isEmpty() || _data.isNull())
      {
        /* qDebug() << "Data is empty."; */
        return -1;
      }

      if((_addr + _data.size()) > __sReadDataArray.size())
      {
        /* qDebug() << "Wrong address."; */
        return -1;
      }

      _data.replace(0, _data.size(), __sReadDataArray.mid(_addr, _data.size()));

      return _data.size();
    };

  LCMemoryReadSet memoryaccess(reader);
  initDataMap();
  addItems(memoryaccess);


  memoryaccess.update();
  /* printData(); */
  /* __slDataMap.remove("Data_05"); */
  /* __slDataMap.remove("Data_03"); */
  /* __slDataMap.remove("Data_09"); */
  /* __slDataMap.remove("Data_01"); */
  /* __slDataMap.remove("Data_08"); */
  /* __slDataMap.remove("Data_00"); */
  /* memoryaccess.remove(__slDataMap["Data_00"]); */
  /* memoryaccess.remove(__slDataMap["Data_01"]); */
  /* memoryaccess.remove(__slDataMap["Data_02"]); */
  /* memoryaccess.remove(__slDataMap["Data_03"]); */
  /* memoryaccess.remove(__slDataMap["Data_04"]); */
  /* memoryaccess.remove(__slDataMap["Data_05"]); */
  /* memoryaccess.update(); */
  /* /1* memoryaccess.update(); *1/ */
  /* memoryaccess.update(); */
  /* memoryaccess.insert(__slDataMap.find("Data_13").value()); */
  /* memoryaccess.insert(__slDataMap.find("Data_14").value()); */
  /* memoryaccess.insert(__slDataMap.find("Data_15").value()); */
  /* memoryaccess.insert(__slDataMap.find("Data_18").value()); */
  /* memoryaccess.update(); */
  __slDataMap.remove("Data_00");
  __slDataMap.remove("Data_00");
  /* memoryaccess.remove(__slDataMap.find("Data_00").value()); */
  /* memoryaccess.remove(__slDataMap.find("Data_01").value()); */
  /* memoryaccess.remove(__slDataMap.find("Data_02").value()); */
  /* memoryaccess.remove(__slDataMap.find("Data_03").value()); */
  /* memoryaccess.insert(__slDataMap.find("Data_02").value()); */
  /* memoryaccess.insert(__slDataMap.find("Data_03").value()); */
  memoryaccess.update();
  memoryaccess.remove(__slDataMap.find("Data_02").value());
  memoryaccess.update();
  /* printData(); */
  return 0;
}

using SPData = QSharedPointer<CTestData>;

//==============================================================================
static void initDataMap()
{
  __slDataMap.insert(QString("Data_00"), SPData(new CTestData(  0, 1)));
  __slDataMap.insert(QString("Data_01"), SPData(new CTestData(  1, 1)));
  __slDataMap.insert(QString("Data_02"), SPData(new CTestData(  2, 1)));
  __slDataMap.insert(QString("Data_03"), SPData(new CTestData(  3, 1)));
  __slDataMap.insert(QString("Data_04"), SPData(new CTestData(  4, 1)));
  __slDataMap.insert(QString("Data_05"), SPData(new CTestData(  5, 1)));
  __slDataMap.insert(QString("Data_06"), SPData(new CTestData(  6, 1)));
  __slDataMap.insert(QString("Data_07"), SPData(new CTestData(  7, 1)));
  __slDataMap.insert(QString("Data_08"), SPData(new CTestData(  8, 1)));
  __slDataMap.insert(QString("Data_09"), SPData(new CTestData(  9, 1)));
  __slDataMap.insert(QString("Data_10"), SPData(new CTestData( 10, 1)));
  __slDataMap.insert(QString("Data_11"), SPData(new CTestData( 11, 1)));
  __slDataMap.insert(QString("Data_12"), SPData(new CTestData( 12, 1)));
  __slDataMap.insert(QString("Data_13"), SPData(new CTestData( 13, 1)));
  __slDataMap.insert(QString("Data_14"), SPData(new CTestData( 14, 1)));
  __slDataMap.insert(QString("Data_15"), SPData(new CTestData( 15, 1)));
  __slDataMap.insert(QString("Data_16"), SPData(new CTestData( 16, 1)));
  __slDataMap.insert(QString("Data_17"), SPData(new CTestData( 17, 1)));
  __slDataMap.insert(QString("Data_18"), SPData(new CTestData( 18, 1)));
  __slDataMap.insert(QString("Data_19"), SPData(new CTestData( 19, 1)));

  __slDataMap.insert(QString("Data_30"), SPData(new CTestData( 30, 1)));
  __slDataMap.insert(QString("Data_31"), SPData(new CTestData( 31, 1)));
  __slDataMap.insert(QString("Data_32"), SPData(new CTestData( 32, 1)));
  __slDataMap.insert(QString("Data_33"), SPData(new CTestData( 33, 1)));
  __slDataMap.insert(QString("Data_34"), SPData(new CTestData( 34, 1)));
  __slDataMap.insert(QString("Data_35"), SPData(new CTestData( 35, 1)));
  __slDataMap.insert(QString("Data_36"), SPData(new CTestData( 36, 1)));
  __slDataMap.insert(QString("Data_37"), SPData(new CTestData( 37, 1)));
  __slDataMap.insert(QString("Data_38"), SPData(new CTestData( 38, 1)));
  __slDataMap.insert(QString("Data_39"), SPData(new CTestData( 39, 1)));
}


//==============================================================================
static void addItems(LCMemoryReadSet& _memorySetAccess)
{
  /* auto it = __slDataMap.find("Data_00"); */
  /* if(it == __slDataMap.end()) */ 
  /* { */
  /*   qDebug() << "Can't find element."; */
  /*   return; */
  /* } */
  _memorySetAccess.insert(__slDataMap.find("Data_00").value());
  _memorySetAccess.insert(__slDataMap.find("Data_01").value());
  /* _memorySetAccess.insert(__slDataMap.find("Data_02").value()); */
  /* _memorySetAccess.insert(__slDataMap.find("Data_03").value()); */
  _memorySetAccess.insert(__slDataMap.find("Data_04").value());
  _memorySetAccess.insert(__slDataMap.find("Data_05").value());
  /* _memorySetAccess.insert(__slDataMap.find("Data_06").value()); */
  /* _memorySetAccess.insert(__slDataMap.find("Data_07").value()); */
  /* _memorySetAccess.insert(__slDataMap.find("Data_08").value()); */
  /* _memorySetAccess.insert(__slDataMap.find("Data_09").value()); */
  /* _memorySetAccess.insert(__slDataMap.find("Data_10").value()); */
  /* _memorySetAccess.insert(__slDataMap.find("Data_11").value()); */

  /* _memorySetAccess.insert(__slDataMap.find("Data_30").value()); */
  /* _memorySetAccess.insert(__slDataMap.find("Data_31").value()); */
  /* _memorySetAccess.insert(__slDataMap.find("Data_32").value()); */
  /* _memorySetAccess.insert(__slDataMap.find("Data_33").value()); */
  /* _memorySetAccess.insert(__slDataMap.find("Data_34").value()); */
  /* _memorySetAccess.insert(__slDataMap.find("Data_35").value()); */
  /* _memorySetAccess.insert(__slDataMap.find("Data_36").value()); */
  /* _memorySetAccess.insert(__slDataMap.find("Data_37").value()); */
  /* _memorySetAccess.insert(__slDataMap.find("Data_38").value()); */
  /* _memorySetAccess.insert(__slDataMap.find("Data_39").value()); */
}

static void printData()
{
  qDebug() << "----------------------print";
  for(auto it = __slDataMap.begin(); it != __slDataMap.end(); it++)
  {
    qDebug() << it.value()->getData().toHex(' ');
  }
}


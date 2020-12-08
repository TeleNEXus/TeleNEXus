#ifndef LCDATAITEMMAP_H_
#define LCDATAITEMMAP_H_

#include <QMap>
#include <QBitArray>
#include <QSharedPointer>

class LCDataItemMap
{
private:

  class CDataItemBase
  {
  public:
    CDataItemBase(){}
    virtual QByteArray getData() = 0;
    virtual int setData(const QByteArray& _data) = 0;
  }; 

  class CDataItemBytes : public CDataItemBase
  {
  private:
    QByteArray mData;
  public:
    CDataItemBytes() = delete;
    CDataItemBytes(const QByteArray& _data):
      mData(_data){}
    virtual QByteArray getData() override { return mData; }
    virtual int setData(const QByteArray& _data) override;
  };

  class CDataItemBits : public CDataItemBase
  {
  private:
    QBitArray mData;
  public:
    CDataItemBits() = delete;
    CDataItemBits(const QBitArray& _data) : mData(_data){}
    virtual QByteArray getData() override;
    virtual int setData(const QByteArray& _data) override;
  };

  QMap<QString, QSharedPointer<CDataItemBase>> mDataMap;
  
public:

  LCDataItemMap();

  void addItem(const QString& _id, const QByteArray& _data);
  void addItem(const QString& _id, const QBitArray& _data);
  int setData(const QString& _id, const QByteArray& _data);
  QByteArray getData(const QString& _id);
};

#endif //LCDATAITEMMAP_H_

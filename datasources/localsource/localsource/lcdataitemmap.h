#ifndef LCDATAITEMMAP_H_
#define LCDATAITEMMAP_H_

#include <QMap>
#include <QBitArray>
#include <QSharedPointer>
#include <QList>

class LCQLocalDataReader;

class LCDataItemMap
{
private:

  using TReadersMap =QMap<QWeakPointer<LCQLocalDataReader>, QString>;

  class CDataItemBase
  {
  protected:
    TReadersMap& edReadersMap;
    QList<QWeakPointer<LCQLocalDataReader>> mReadersList;
    void notifyReaders(const QByteArray& _data);

  public:
    CDataItemBase(TReadersMap& _rm): edReadersMap(_rm){}

    void connectReader(QSharedPointer<LCQLocalDataReader> _sw_reader);
    void disconnectReader(QSharedPointer<LCQLocalDataReader> _sw_reader);

    virtual QByteArray getData() = 0;
    virtual int setData(const QByteArray& _data) = 0;
  }; 

  class CDataItemBytes : public CDataItemBase
  {
  private:
    QByteArray mData;
  public:
    CDataItemBytes() = delete;
    CDataItemBytes(const QByteArray& _data, TReadersMap& _rm):
      CDataItemBase(_rm),
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
    CDataItemBits(const QBitArray& _data, TReadersMap& _rm) :
      CDataItemBase(_rm),
      mData(_data){}
    virtual QByteArray getData() override;
    virtual int setData(const QByteArray& _data) override;
  };

  QMap<QString, QSharedPointer<CDataItemBase>> mDataMap;
  QMap<QWeakPointer<LCQLocalDataReader>, QString> mReadersMap;
  
public:

  LCDataItemMap();

  void addItem(const QString& _id, const QByteArray& _data);
  void addItem(const QString& _id, const QBitArray& _data);

  int setData(const QString& _id, const QByteArray& _data);
  QByteArray getData(const QString& _id);

  void connectReader(QSharedPointer<LCQLocalDataReader> _sp_reader);
  void disconnectReader(QSharedPointer<LCQLocalDataReader> _sp_reader);
};

#endif //LCDATAITEMMAP_H_

#ifndef LCDATAITEMMAP_H_
#define LCDATAITEMMAP_H_

#include <QMap>
#include <QBitArray>
#include <QSharedPointer>
#include <QList>

class LCQLocalDataReader;
class LCQLocalDataWriter;

class LCDataItemMap
{
private:

  using TReadersMap =QMap<QWeakPointer<LCQLocalDataReader>, QString>;

  class CDataItemBase
  {
  protected:
    QList<QWeakPointer<LCQLocalDataReader>> mReadersList;
    void notifyReaders(const QByteArray& _data);

  public:
    CDataItemBase(){}

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
    CDataItemBits(const QBitArray& _data) :
      mData(_data){}
    virtual QByteArray getData() override;
    virtual int setData(const QByteArray& _data) override;
  };

  QMap<QString, QSharedPointer<CDataItemBase>> mDataMap;
  /* QMap<QString, QWeakPointer<LCQLocalDataReader>> mReadersMap; */
  
public:

  LCDataItemMap();

  void addItem(const QString& _id, const QByteArray& _data);
  void addItem(const QString& _id, const QBitArray& _data);

  void readData(QSharedPointer<LCQLocalDataReader> _sp_reader);
  void writeData(
      QSharedPointer<LCQLocalDataWriter> _sp_writer, 
      const QByteArray& _data);

  void connectReader(QSharedPointer<LCQLocalDataReader> _sp_reader);
  void disconnectReader(QSharedPointer<LCQLocalDataReader> _sp_reader);
};

#endif //LCDATAITEMMAP_H_

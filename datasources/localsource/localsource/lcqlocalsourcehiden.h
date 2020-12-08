#ifndef LCLOCALSOURCEHIDEN_H_
#define LCLOCALSOURCEHIDEN_H_

#include <QObject>
#include "lcdataitemmap.h"

class LCQLocalDataReader;
class LCQLocalDataWriter;
class LIRemoteDataReader;
class LIRemoteDataWriter;

class LCQLocalSourceHiden final : public QObject 
{
private:

  LCDataItemMap mDataMap;
public:

  explicit LCQLocalSourceHiden();

  virtual ~LCQLocalSourceHiden();

  void addItem(const QString& _id, const QByteArray& _data); 
  void addItem(const QString& _id, const QBitArray& _data); 

private:

  void connectReader(LCQLocalDataReader* _p_reader);
  void disconnectReader(LCQLocalDataReader* _p_reader);
  void read(LCQLocalDataReader* _p_reader);
  void write(const QByteArray& _data, LCQLocalDataWriter* _writer);

  virtual void customEvent(QEvent* _event) override;

  friend class LCQLocalDataReader;
  friend class LCQLocalDataWriter;
};

#endif //LCLOCALSOURCEHIDEN_H_

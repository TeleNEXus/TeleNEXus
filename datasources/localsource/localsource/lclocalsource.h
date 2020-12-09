#ifndef LCLOCALSOURCE_H_
#define LCLOCALSOURCE_H_

#include "LIRemoteDataSource.h"
#include <QSharedPointer>

class LCLocalDataSource : public LIRemoteDataSource
{
private:
  void* mpData;
  LCLocalDataSource();
  LCLocalDataSource(const LCLocalDataSource&) = delete;
  LCLocalDataSource& operator=(const LCLocalDataSource&) = delete;
public:
  virtual ~LCLocalDataSource();

  static QSharedPointer<LCLocalDataSource> create();

  void addByteItem(const QString& _dataName, const QByteArray& _data); 
  void addBitItem(const QString& _dataName, const QBitArray& _data); 

  virtual QSharedPointer<LIRemoteDataReader> createReader(
      const QString& _dataName,
      QWeakPointer<LIRemoteDataReadListener> _readListener)  override;

  virtual QSharedPointer<LIRemoteDataWriter> createWriter(
      const QString& _dataName,
      QWeakPointer<LIRemoteDataWriteListener> _writeListener)  override;

};

#endif //LCLOCALSOURCE_H_

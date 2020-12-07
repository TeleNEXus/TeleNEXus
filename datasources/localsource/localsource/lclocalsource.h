#ifndef LCLOCALSOURCE_H_
#define LCLOCALSOURCE_H_

#include "LIRemoteDataSource.h"

class LCLocalDataSource : public LIRemoteDataSource
{
public:
  LCLocalDataSource();
  virtual ~LCLocalDataSource();
  virtual QSharedPointer<LIRemoteDataReader> createReader()  override;
  virtual QSharedPointer<LIRemoteDataWriter> createWriter()  override;
};

#endif //LCLOCALSOURCE_H_

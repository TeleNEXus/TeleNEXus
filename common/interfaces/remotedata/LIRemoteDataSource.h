#ifndef LIREMOTEDATASOURCE_H
#define LIREMOTEDATASOURCE_H

#include <QSharedPointer>

enum class LERemoteDataStatus
{
    DS_UNDEF, // Первичной инициализации статуса операции с данными 
              // и при попытке чтения несуществующих данных.
    DS_OK,    // Операция с данными корректна.
    DS_WRONG  // Операция с данными ошибочна.
};

class LIRemoteDataReader;
class LIRemoteDataWriter;
class LIRemoteDataReadListener;
class LIRemoteDataWriteListener;

class LIRemoteDataSource
{
public:
    LIRemoteDataSource(){}
    virtual ~LIRemoteDataSource(){}

    virtual QSharedPointer<LIRemoteDataReader> createReader(
        const QString& _dataName,
        QWeakPointer<LIRemoteDataReadListener> _readListener) = 0;

    virtual QSharedPointer<LIRemoteDataWriter> createWriter(
        const QString& _dataName,
        QWeakPointer<LIRemoteDataWriteListener> _writeListener) = 0;

};

#endif // LIREMOTEDATASOURCE_H

#ifndef LIREMOTEDATASOURCE_H
#define LIREMOTEDATASOURCE_H

#include <QSharedPointer>
#include <QByteArray>

#include <functional>

enum class LERemoteDataStatus
{
    DS_UNDEF, // Первичной инициализации статуса операции с данными 
              // и при попытке чтения несуществующих данных.
    DS_OK,    // Операция с данными корректна.
    DS_WRONG  // Операция с данными ошибочна.
};

class LIRemoteDataReader;
class LIRemoteDataWriter;

using LTWriteListener = std::function<void(LERemoteDataStatus)>;
using LTReadAction = std::function<void(QSharedPointer<QByteArray>, LERemoteDataStatus)>; 

class LIRemoteDataSource
{
public:


    LIRemoteDataSource(){}
    virtual ~LIRemoteDataSource(){}

    virtual QSharedPointer<LIRemoteDataReader> createReader(
        const QString& _dataName,
        LTReadAction _readAction) = 0;

    virtual QSharedPointer<LIRemoteDataWriter> createWriter(
        const QString& _dataName,
        LTWriteListener _listener) = 0;


    /* //Блокирующие вызовы. */
    /* virtual QByteArray read( */
    /*     const QString& _dataName, */ 
    /*     LERemoteDataStatus* _status = nullptr) = 0; */

    /* virtual LERemoteDataStatus write( */
    /*     const QString& _dataName, */ 
    /*     const QByteArray& _data) = 0; */

    /* //Асинхронные вызовы. */
    /* virtual void read( */
    /*     const QString& _dataName, */ 
    /*     std::function<void( */
    /*       QSharedPointer<QByteArray>, LERemoteDataStatus)> _listener) = 0; */

    /* virtual LERemoteDataStatus write( */
    /*     const QString& _dataName, */ 
    /*     std::function<void(LERemoteDataStatus)> _isWrite) = 0; */
};

#endif // LIREMOTEDATASOURCE_H

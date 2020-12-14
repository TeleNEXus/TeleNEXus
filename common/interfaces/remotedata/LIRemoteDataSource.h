#ifndef LIREMOTEDATASOURCE_H
#define LIREMOTEDATASOURCE_H

#include <QSharedPointer>
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
class LIRemoteDataReadListener;
/* class LIRemoteDataWriteListener; */

using LTWriteListener = std::function<void(LERemoteDataStatus)>;


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
        LTWriteListener _listener = [](LERemoteDataStatus _status){Q_UNUSED(_status);}) = 0;


    /* virtual QSharedPointer<LIRemoteDataReader> createReader( */
    /*     const QString& _dataName, */
    /*     QWeakPointer<LIRemoteDataReadListener> _readListener) = 0; */

    /* virtual QSharedPointer<LIRemoteDataWriter> createWriter( */
    /*     const QString& _dataName, */
    /*     QWeakPointer<LIRemoteDataWriteListener> _writeListener) = 0; */

    /* virtual QSharedPointer<LIRemoteDataReader> createReader( */
    /*     const QString& _dataName, */
    /*     std::function<void( */
    /*       QSharedPointer<QByteArray>,LERemoteDataStatus)> _listener) = 0; */

    /* virtual QSharedPointer<LIRemoteDataWriter> createWriter( */
    /*     const QString& _dataName, */
    /*     std::function<void(LERemoteDataStatus)> _isWrite) = 0; */

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

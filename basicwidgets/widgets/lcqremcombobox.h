#ifndef LCQEXTLABEL_H
#define LCQEXTLABEL_H

#include <QComboBox>
#include <functional>
#include "LIRemoteDataReader.h"
#include "LIRemoteDataWriter.h"
#include "lqextendevent.h"
#include "lcstringdataformatterbase.h"

#include "LIRemoteDataReadListener.h"
#include "LIRemoteDataWriteListner.h"


class LCQRemComboBox : public QComboBox
{
    Q_OBJECT
private:
    
    //--------------------------------------------------------------------------
    class CReadListener : public LIRemoteDataReadListener
    {
    private:
        LCQRemComboBox& mOwner;
    public:
        CReadListener(LCQRemComboBox& _owner);
        virtual ~CReadListener(){}
        virtual void dataIsRead(QSharedPointer<QByteArray> _data, 
                LERemoteDataStatus status) override;
    };

    //--------------------------------------------------------------------------
    class CWriteListener : public LIRemoteDataWriteListner
    {
    public:
        CWriteListener(){}
        virtual ~CWriteListener(){}
        virtual void dataIsWrite(LERemoteDataStatus _status) = 0;
    };

    QSharedPointer<CReadListener>               mDataReadListener;
    QSharedPointer<LIRemoteDataReader>          mDataReader;
    QSharedPointer<LIRemoteDataWriter>          mDataWriter;
    QSharedPointer<LCStringDataFormatterBase>   mFormatter;
    QMetaObject::Connection                     mIndexChangeConnection;
public:
    explicit LCQRemComboBox(QWidget* _parent = nullptr);

    explicit LCQRemComboBox(QString _text, QWidget* _parent = nullptr);

    explicit LCQRemComboBox(    
            const QString&                              _dataNameRead,
            const QString&                              _dataNameWrite,
            QSharedPointer<LIRemoteDataSource>          _dataSource,
            QSharedPointer<LCStringDataFormatterBase>   _formatter,
            QWidget* _parent = nullptr);
    virtual ~LCQRemComboBox();
    virtual bool event(QEvent *e) override;

};

#endif // LCQEXTLABEL_H

#ifndef LCQREMLINEEDIT_H
#define LCQREMLINEEDIT_H

#include <QLineEdit>

#include "lqextendevent.h"
#include "lcstringdataformatterbase.h"
#include "LIRemoteDataReader.h"
#include "LIRemoteDataWriter.h"
#include "LIRemoteDataReadListener.h"
#include "LIRemoteDataWriteListner.h"
#include "LIRemoteDataSource.h"

class LCQRemLineEdit : public QLineEdit
{
    Q_OBJECT

private:

    class CReadListener : public LIRemoteDataReadListener
    {
    private:
        LCQRemLineEdit& mLineEdit;
        bool mFlagActive;
    public:
        CReadListener(LCQRemLineEdit& _lineEdit);
        virtual ~CReadListener(){}
        virtual void dataIsRead(QSharedPointer<QByteArray> _data, LERemoteDataStatus status) override;
        void setActive(bool _flag){mFlagActive = _flag;}
    };

    class CWriteListener : public LIRemoteDataWriteListner
    {
    private:
        LCQRemLineEdit& mLineEdit;
    public:
        CWriteListener(LCQRemLineEdit& _lineEdit);
        virtual ~CWriteListener(){}
        virtual void dataIsWrite(LERemoteDataStatus _status) override;
    };

    QString mDataName;

    QSharedPointer<LIRemoteDataReader>  mDataReader;
    QSharedPointer<LIRemoteDataWriter>  mDataWriter;

    QSharedPointer<CReadListener>   mReadListener;
    QSharedPointer<CWriteListener>  mWriteListener;

    QSharedPointer<LCStringDataFormatterBase> mFormatter;

public:
    explicit LCQRemLineEdit(const QString& _dataName,
                            QSharedPointer<LIRemoteDataSource> _dataSource,
                            QSharedPointer<LCStringDataFormatterBase> _formatter,
                            QWidget* _parent = nullptr);

    explicit LCQRemLineEdit(const QString& _dataNameRead,
                            const QString& _dataNameWrite,
                            QSharedPointer<LIRemoteDataSource> _dataSource,
                            QSharedPointer<LCStringDataFormatterBase> _formatter,
                            QWidget* _parent = nullptr);

    virtual ~LCQRemLineEdit();
    void setActive(bool _flag);
protected:
    virtual void keyPressEvent(QKeyEvent *_ev) override;
    virtual void focusInEvent(QFocusEvent *) override;
    virtual void focusOutEvent(QFocusEvent *) override;
    virtual bool event(QEvent *e) override;
};

#endif // LCQREMLINEEDIT_H

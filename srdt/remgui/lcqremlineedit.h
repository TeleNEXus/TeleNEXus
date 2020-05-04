#ifndef LCQREMLINEEDIT_H
#define LCQREMLINEEDIT_H

#include <QLineEdit>

#include "lqextendevent.h"
#include "lcstringdataformatterbase.h"
#include "LCRemoteDataReaderInterface.h"
#include "LCRemoteDataWriterInterface.h"
#include "LCRemoteDataReadListenerInterface.h"
#include "LCRemoteDataWriteListnerInterface.h"
#include "LCRemoteDataSourceInterface.h"

namespace remgui
{

class LCQRemLineEdit : public QLineEdit
{
    Q_OBJECT

private:

    class CReadListener : public LCRemoteDataReadListenerInterface
    {
    private:
        LCQRemLineEdit& mLineEdit;
        bool mFlagActive;
    public:
        CReadListener(LCQRemLineEdit& _lineEdit);
        virtual ~CReadListener(){}
        virtual void dataIsRead(QSharedPointer<QByteArray> _data, ERemoteDataStatus status) override;
        void setActive(bool _flag){mFlagActive = _flag;}
    };

    class CWriteListener : public LCRemoteDataWriteListnerInterface
    {
    private:
        LCQRemLineEdit& mLineEdit;
    public:
        CWriteListener(LCQRemLineEdit& _lineEdit);
        virtual ~CWriteListener(){}
        virtual void dataIsWrite(ERemoteDataStatus _status) override;
    };

    QString mDataName;

    QSharedPointer<LCRemoteDataReaderInterface>  mDataReader;
    QSharedPointer<LCRemoteDataWriterInterface>  mDataWriter;

    QSharedPointer<CReadListener>   mReadListener;
    QSharedPointer<CWriteListener>  mWriteListener;

    QSharedPointer<LCStringDataFormatterBase> mFormatter;

public:
    explicit LCQRemLineEdit(const QString& _dataName,
                            QSharedPointer<LCRemoteDataSourceInterface> _dataSource,
                            QSharedPointer<LCStringDataFormatterBase> _formatter,
                            QWidget* _parent = nullptr);

    explicit LCQRemLineEdit(const QString& _dataNameRead,
                            const QString& _dataNameWrite,
                            QSharedPointer<LCRemoteDataSourceInterface> _dataSource,
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
}

#endif // LCQREMLINEEDIT_H

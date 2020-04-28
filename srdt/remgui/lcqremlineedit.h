#ifndef LCQREMLINEEDIT_H
#define LCQREMLINEEDIT_H

#include <QLineEdit>

#include "lqextendevent.h"
#include "lcdatastrformatbase.h"
#include "LCRemoteDataReaderInterface.h"
#include "LCRemoteDataWriterInterface.h"
#include "LCRemoteDataReadListnerInterface.h"
#include "LCRemoteDataWriteListnerInterface.h"
#include "LCRemoteDataSourceInterface.h"

namespace remgui
{

class LCQRemLineEdit : public QLineEdit
{
    Q_OBJECT

private:

    class CReadListener : public LCRemoteDataReadListnerInterface
    {
    private:
        LCQRemLineEdit& mLineEdit;
        bool mFlagActive;
    public:
        CReadListener(LCQRemLineEdit& _lineEdit);
        virtual void dataIsRead(QSharedPointer<QByteArray> _data, ERemoteDataStatus status) override;
        void setActive(bool _flag){mFlagActive = _flag;}
    };

    class CWriteListener : public LCRemoteDataWriteListnerInterface
    {
    private:
        LCQRemLineEdit& mLineEdit;
    public:
        CWriteListener(LCQRemLineEdit& _lineEdit);
        virtual void dataIsWrite(ERemoteDataStatus _status) override;
    };

    QString mDataName;

    QSharedPointer<LCRemoteDataReaderInterface>  mDataReader;
    QSharedPointer<LCRemoteDataWriterInterface>  mDataWriter;

    QSharedPointer<CReadListener>   mReadListener;
    QSharedPointer<CWriteListener>  mWriteListener;

    QSharedPointer<LCDataStrFormatBase> mFormatter;

public:
    explicit LCQRemLineEdit(const QString& _dataName,
                            QSharedPointer<LCRemoteDataSourceInterface> _dataSource,
                            QSharedPointer<LCDataStrFormatBase> _formatter,
                            QWidget* _parent = nullptr);

    explicit LCQRemLineEdit(const QString& _dataNameRead,
                            const QString& _dataNameWrite,
                            QSharedPointer<LCRemoteDataSourceInterface> _dataSource,
                            QSharedPointer<LCDataStrFormatBase> _formatter,
                            QWidget* _parent = nullptr);

    void setActive(bool _flag);
protected:
    virtual void keyPressEvent(QKeyEvent *_ev) override;
    virtual void focusInEvent(QFocusEvent *) override;
    virtual void focusOutEvent(QFocusEvent *) override;
    virtual bool event(QEvent *e) override;
};
}

#endif // LCQREMLINEEDIT_H

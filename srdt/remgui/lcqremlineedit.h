#ifndef LCQREMLINEEDIT_H
#define LCQREMLINEEDIT_H

#include "lqextendevent.h"
#include "lcqremotedatareader.h"
#include "lcqremotedatawriter.h"
#include "lcdatastrformatbase.h"

#include <QLineEdit>

namespace remgui
{

class LCQRemLineEdit : public QLineEdit
{
    Q_OBJECT

private:
    LCQRemoteDataReader mDataReader;
    LCQRemoteDataWriter mDataWriter;
    QSharedPointer<LCDataStrFormatBase> mFormatter;
    QMetaObject::Connection mConnection;
public:
    explicit LCQRemLineEdit(const QString& _dataName,
                            QWeakPointer<LCQRemoteDataSourceBase> _dataSource,
                            QSharedPointer<LCDataStrFormatBase> _formatter,
                            QWidget* _parent = nullptr);

    explicit LCQRemLineEdit(const QString& _dataNameRead,
                            const QString& _dataNameWrite,
                            QWeakPointer<LCQRemoteDataSourceBase> _dataSource,
                            QSharedPointer<LCDataStrFormatBase> _formatter,
                            QWidget* _parent = nullptr);

    void setActive(bool _flag);
protected:
    virtual void updateText(QSharedPointer<QByteArray> _data,
                            LCQRemoteDataSourceBase::EDataStatus _status);
    virtual void keyPressEvent(QKeyEvent *_ev) override;
    virtual void focusInEvent(QFocusEvent *) override;
    virtual void focusOutEvent(QFocusEvent *) override;
    virtual bool event(QEvent *e) override;
};
}

#endif // LCQREMLINEEDIT_H

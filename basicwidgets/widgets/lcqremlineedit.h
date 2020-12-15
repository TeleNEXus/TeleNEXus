#ifndef LCQREMLINEEDIT_H
#define LCQREMLINEEDIT_H

#include <QLineEdit>

#include "lqextendevent.h"
#include "LIDataFormatter.h"
#include "LIRemoteDataReader.h"
#include "LIRemoteDataWriter.h"
#include "LIRemoteDataSource.h"


class LCQRemLineEdit : public QLineEdit
{
    Q_OBJECT

private:

    QString mDataName;

    QSharedPointer<LIRemoteDataReader>  mDataReader;
    QSharedPointer<LIRemoteDataWriter>  mDataWriter;
    QSharedPointer<LIDataFormatter> mFormatter;

public:

    explicit LCQRemLineEdit(const QString& _dataNameRead,
                            const QString& _dataNameWrite,
                            QSharedPointer<LIRemoteDataSource> _dataSource,
                            QSharedPointer<LIDataFormatter> _formatter,
                            QWidget* _parent = nullptr);

    virtual ~LCQRemLineEdit();
protected:
    bool mFlagUpdateOn = false;
    void setActive(bool _flag);
    virtual void keyPressEvent(QKeyEvent *_ev) override;
    virtual void focusInEvent(QFocusEvent *) override;
    virtual void focusOutEvent(QFocusEvent *) override;
    virtual bool event(QEvent *e) override;
};

#endif // LCQREMLINEEDIT_H

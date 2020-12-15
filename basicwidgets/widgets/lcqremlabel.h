#ifndef LCQEXTLABEL_H
#define LCQEXTLABEL_H

#include "lremgui.h"
#include "lqextendevent.h"

#include "LIDataFormatter.h"
#include "LIRemoteDataReader.h"
#include "LIRemoteDataSource.h"

#include <QLabel>

class LCQRemLabel : public QLabel
{
  Q_OBJECT
private:

    QString mDataName;
    QSharedPointer<LIRemoteDataReader>  mDataReader;
    QSharedPointer<LIDataFormatter> mFormatter;

public:
    explicit LCQRemLabel(QWidget* _parent = nullptr);

    explicit LCQRemLabel(QString _text, QWidget* _parent = nullptr);

    explicit LCQRemLabel(   
        const QString&                      _dataName,
        QSharedPointer<LIRemoteDataSource>  _dataSource,
        QSharedPointer<LIDataFormatter>     _formatter,
        QWidget* _parent = nullptr);

    virtual ~LCQRemLabel();
    virtual bool event(QEvent *e) override;
private:
    bool mFlagActive;
    void setActive(bool _flag);
};

#endif // LCQEXTLABEL_H

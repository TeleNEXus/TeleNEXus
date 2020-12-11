#ifndef LCQEXTLABEL_H
#define LCQEXTLABEL_H

#include "lremgui.h"
#include "lqextendevent.h"

#include "LIDataFormatter.h"
#include "LIRemoteDataReader.h"
#include "LIRemoteDataReadListener.h"
#include "LIRemoteDataSource.h"

#include <QLabel>

class LCQRemLabel : public QLabel
{
  Q_OBJECT
private:

    class CReadListener : public LIRemoteDataReadListener
  {
  private:
    LCQRemLabel& mLabel;
    bool mFlagActive;
  public:
    CReadListener(LCQRemLabel& _label);
    virtual ~CReadListener(){}
    virtual void dataIsRead(QSharedPointer<QByteArray> _data, 
        LERemoteDataStatus status) override;
    void setActive(bool _flag){mFlagActive = _flag;}
  };

    QString mDataName;
    QSharedPointer<LIRemoteDataReader>  mDataReader;
    QSharedPointer<LIDataFormatter> mFormatter;
    QSharedPointer<CReadListener> mDataListener;

public:
    explicit LCQRemLabel(QWidget* _parent = nullptr);

    explicit LCQRemLabel(QString _text, QWidget* _parent = nullptr);

    explicit LCQRemLabel(   
        const QString&                      _dataName,
        QSharedPointer<LIRemoteDataSource>  _dataSource,
        QSharedPointer<LIDataFormatter>     _formatter,
        QWidget* _parent = nullptr);

    virtual ~LCQRemLabel();
    void setActive(bool _flag);
    virtual bool event(QEvent *e) override;
};

#endif // LCQEXTLABEL_H

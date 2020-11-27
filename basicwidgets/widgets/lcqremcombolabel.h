#ifndef LCQREMCOMBOLABEL_H
#define LCQREMCOMBOLABEL_H

#include <QStackedWidget>
#include <qnamespace.h>
#include "lremgui.h"
#include "lqextendevent.h"

#include "LIDataFormatter.h"
#include "LIRemoteDataReader.h"
#include "LIRemoteDataReadListener.h"
#include "LIRemoteDataSource.h"
class LIMovieAccess;

class LCQRemComboLabel : public QStackedWidget 
{
  Q_OBJECT
private:

    class CReadListener : public LIRemoteDataReadListener
  {
  private:
    LCQRemComboLabel& mLabel;
  public:
    CReadListener(LCQRemComboLabel& _label);
    virtual ~CReadListener(){}
    virtual void dataIsRead(QSharedPointer<QByteArray>  _data, 
        LERemoteDataStatus          _status) override;
  };

    void* mpOwnData;    //Собственные данные.
    QString mDataName;
    QSharedPointer<LIRemoteDataReader>  mDataReader;
    QSharedPointer<LIDataFormatter> mspFormatter;
    QSharedPointer<CReadListener> mDataListener;
public:
    explicit LCQRemComboLabel(   
        const QString&                              _dataName,
        QSharedPointer<LIRemoteDataSource>          _dataSource,
        QSharedPointer<LIDataFormatter>             _formatter,
        QWidget* _parent = nullptr);

    virtual ~LCQRemComboLabel();
    void setActive(bool _flag);
    virtual bool event(QEvent *e) override;

    void addItem(QWidget* _widget, const QString&  _val);
    void addItemUndef(QWidget* _widget);
    void addItemWrong(QWidget* _widget);
};

#endif // LCQREMCOMBOLABEL_H

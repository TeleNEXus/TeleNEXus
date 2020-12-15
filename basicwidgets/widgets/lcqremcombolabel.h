#ifndef LCQREMCOMBOLABEL_H
#define LCQREMCOMBOLABEL_H

#include <QStackedWidget>
#include <qnamespace.h>
#include "lqextendevent.h"

#include "LIDataFormatter.h"
#include "LIRemoteDataReader.h"
#include "LIRemoteDataSource.h"
class LIMovieAccess;

class LCQRemComboLabel : public QStackedWidget 
{
  Q_OBJECT
private:
    void* mpOwnData;    //Собственные данные.
    QSharedPointer<LIRemoteDataReader>  mDataReader;
    QSharedPointer<LIDataFormatter> mspFormatter;
public:

    explicit LCQRemComboLabel(   
        const QString&                      _dataName,
        QSharedPointer<LIRemoteDataSource>  _dataSource,
        QSharedPointer<LIDataFormatter>     _formatter,
        QWidget* _parent = nullptr);

    virtual ~LCQRemComboLabel();

    void addItem(QWidget* _widget, const QString&  _val);
    void addItemUndef(QWidget* _widget);
    void addItemWrong(QWidget* _widget);
private:
    bool mFlagActive = false;
    void setActive(bool _flag);
    virtual bool event(QEvent *e) override;
};

#endif // LCQREMCOMBOLABEL_H

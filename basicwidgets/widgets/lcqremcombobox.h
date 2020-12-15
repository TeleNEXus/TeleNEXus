#ifndef LCQEXTLABEL_H
#define LCQEXTLABEL_H

#include <QComboBox>
#include <functional>
#include "LIRemoteDataReader.h"
#include "LIRemoteDataWriter.h"
#include "lqextendevent.h"

#include "LIDataFormatter.h"
#include "LIRemoteDataSource.h"


class LCQRemComboBox : public QComboBox
{
    Q_OBJECT
private:
    QSharedPointer<LIRemoteDataReader>   mDataReader;
    QSharedPointer<LIRemoteDataWriter>   mDataWriter;
    QSharedPointer<LIDataFormatter>      mFormatter;
    bool mFlagPopupOn; //Флаг состояния всплывающего меню.

public:
    explicit LCQRemComboBox(QWidget* _parent = nullptr) = delete;

    explicit LCQRemComboBox(    
            const QString&                      _dataNameRead,
            const QString&                      _dataNameWrite,
            QSharedPointer<LIRemoteDataSource>  _dataSource,
            QSharedPointer<LIDataFormatter>     _formatter,
            QWidget* _parent = nullptr);
    virtual ~LCQRemComboBox();
    virtual bool event(QEvent *e) override;
private:
    virtual void showPopup(void) override;
    virtual void hidePopup(void) override;
};

#endif // LCQEXTLABEL_H

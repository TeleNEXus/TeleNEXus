#ifndef LCQEXTLABEL_H
#define LCQEXTLABEL_H

#include <QComboBox>
#include "lqextendevent.h"
#include "lcstringdataformatterbase.h"

#include "LIRemoteDataReader.h"
#include "LIRemoteDataReadListener.h"
#include "LIRemoteDataSource.h"


class LCQRemComboBox : public QComboBox
{
    Q_OBJECT
private:

    class CReadListener : public LIRemoteDataReadListener
    {
    private:
        LCQRemComboBox& mComboBox;
    public:
        CReadListener(LCQRemComboBox& _label);
        virtual ~CReadListener(){}
        virtual void dataIsRead(QSharedPointer<QByteArray> _data, LERemoteDataStatus status) override;
    };

    QString mDataName;
    QSharedPointer<LIRemoteDataReader>  mDataReader;
    QSharedPointer<LCStringDataFormatterBase> mFormatter;
    QSharedPointer<CReadListener> mDataListener;

public:
    explicit LCQRemComboBox(QWidget* _parent = nullptr);

    explicit LCQRemComboBox(QString _text, QWidget* _parent = nullptr);

    explicit LCQRemComboBox(   const QString&                              _dataName,
                            QSharedPointer<LIRemoteDataSource>          _dataSource,
                            QSharedPointer<LCStringDataFormatterBase>   _formatter,
                            QWidget* _parent = nullptr);
    virtual ~LCQRemComboBox();

    void setActive(bool _flag);

//    void setSource( const QString& _dataName,
//                    QWeakPointer<LCRemoteDataSourceInterface>   _dataSource,
//                    QSharedPointer<LCStringDataFormatterBase>         _formatter);

    virtual bool event(QEvent *e) override;

};

#endif // LCQEXTLABEL_H

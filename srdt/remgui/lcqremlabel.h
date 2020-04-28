#ifndef LCQEXTLABEL_H
#define LCQEXTLABEL_H

#include <QLabel>
#include "lremgui.h"
#include "lqextendevent.h"
#include "lcdatastrformatbase.h"

#include "LCRemoteDataReaderInterface.h"
#include "LCRemoteDataReadListnerInterface.h"
#include "LCRemoteDataSourceInterface.h"


namespace remgui
{
class LCQRemLabel : public QLabel
{
    Q_OBJECT
private:

    class CReadListener : public LCRemoteDataReadListnerInterface
    {
    private:
        LCQRemLabel& mLabel;
    public:
        CReadListener(LCQRemLabel& _label);
        virtual void dataIsRead(QSharedPointer<QByteArray> _data, ERemoteDataStatus status) override;
    };

    QString mDataName;
    QSharedPointer<LCRemoteDataReaderInterface>  mDataReader;
    QSharedPointer<LCDataStrFormatBase> mFormatter;
    QSharedPointer<CReadListener> mDataListener;

public:
    explicit LCQRemLabel(QWidget* _parent = nullptr);

    explicit LCQRemLabel(QString _text, QWidget* _parent = nullptr);

    explicit LCQRemLabel(   const QString&                                  _dataName,
                            QSharedPointer<LCRemoteDataSourceInterface>     _dataSource,
                            QSharedPointer<LCDataStrFormatBase>             _formatter,
                            QWidget* _parent = nullptr);
    virtual ~LCQRemLabel();

    void setActive(bool _flag);

    void setSource( const QString& _dataName,
                    QWeakPointer<LCRemoteDataSourceInterface>   _dataSource,
                    QSharedPointer<LCDataStrFormatBase>         _formatter);

    virtual bool event(QEvent *e) override;

signals:

};

}//namespace remgui
#endif // LCQEXTLABEL_H

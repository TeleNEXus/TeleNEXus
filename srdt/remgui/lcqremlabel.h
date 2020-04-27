#ifndef LCQEXTLABEL_H
#define LCQEXTLABEL_H
#include <QLabel>
#include "lremgui.h"
#include "lqextendevent.h"
#include "lcqremotedatareader.h"
#include "lcdatastrformatbase.h"

namespace remgui
{
class LCQRemLabel : public QLabel
{
    Q_OBJECT
private:

    QString mDataName;
    LCQRemoteDataReader                 mDataReader;
    QSharedPointer<LCDataStrFormatBase> mFormatter;

public:
    explicit LCQRemLabel(QWidget* _parent = nullptr);

    explicit LCQRemLabel(QString _text, QWidget* _parent = nullptr);

    explicit LCQRemLabel(   const QString& _dataName,
                            QWeakPointer<LCQRemoteDataSourceBase>   _dataSource,
                            QSharedPointer<LCDataStrFormatBase>    _formatter,
                            QWidget* _parent = nullptr);
    virtual ~LCQRemLabel();

    void setActive(bool _flag);

    void setSource( const QString& _dataName,
                    QWeakPointer<LCQRemoteDataSourceBase>   _dataSource,
                    QSharedPointer<LCDataStrFormatBase>    _formatter);

    virtual bool event(QEvent *e) override;

signals:

};

}//namespace remgui
#endif // LCQEXTLABEL_H

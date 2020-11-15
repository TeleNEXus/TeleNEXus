#ifndef LCQREMCOMBOLABEL_H
#define LCQREMCOMBOLABEL_H

#include <QLabel>
#include <qnamespace.h>
#include "lremgui.h"
#include "lqextendevent.h"
#include "lcstringdataformatterbase.h"

#include "LIRemoteDataReader.h"
#include "LIRemoteDataReadListener.h"
#include "LIRemoteDataSource.h"
class LIMovieAccess;

class LCQRemComboLabel : public QLabel
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

    QSize mSizeHint;
    void* mpOwnData;    //Собственные данные.
    QString mDataName;
    QSharedPointer<LIRemoteDataReader>  mDataReader;
    QSharedPointer<LCStringDataFormatterBase> mspFormatter;
    QSharedPointer<CReadListener> mDataListener;
private:
    explicit LCQRemComboLabel(QWidget* _parent = nullptr);
    explicit LCQRemComboLabel(QString _text, QWidget* _parent = nullptr);
public:
    explicit LCQRemComboLabel(   
        const QString&                              _dataName,
        QSharedPointer<LIRemoteDataSource>          _dataSource,
        QSharedPointer<LCStringDataFormatterBase>   _formatter,
        QWidget* _parent = nullptr);

    virtual ~LCQRemComboLabel();
    void setActive(bool _flag);
    virtual bool event(QEvent *e) override;

    void addItem(
        const QString&  _text, 
        const QString&  _val, 
        const QFont&    _font,
        const QPalette& _palette,
        Qt::Alignment _alignment);

    void addItem(
        QSharedPointer<LIMovieAccess> _movieAccess, 
        const QString& _val, 
        const QPalette& _palette,
        Qt::Alignment _alignment);
    /* void addItem(const QPixmap& _pixmap, const QString& _val); */
    virtual QSize sizeHint() const override{return mSizeHint;}
private:
    void setSizeHint(const QSize& _size);
};

#endif // LCQREMCOMBOLABEL_H

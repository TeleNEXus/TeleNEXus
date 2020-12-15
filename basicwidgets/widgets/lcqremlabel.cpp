#include "lcqremlabel.h"
#include <QCoreApplication>
#include <QDebug>
#include <qnamespace.h>

//==============================================================================LCQRemLabel
LCQRemLabel::LCQRemLabel(QWidget* _parent) : QLabel(_parent), mFlagActive(false)
{
  setText("LCQRemLabel");
  setEnabled(false);
}

LCQRemLabel::LCQRemLabel(QString _text, QWidget* _parent) : 
  QLabel(_text, _parent),
  mFlagActive(false)
{
  setEnabled(false);
}

LCQRemLabel::LCQRemLabel(const QString& _dataName,
    QSharedPointer<LIRemoteDataSource> _dataSource,
    QSharedPointer<LIDataFormatter> _formatter,
    QWidget* _parent) :    QLabel(_parent),
  mFormatter(_formatter),
  mFlagActive(false)
{
  setEnabled(false);
  QString str = "Undef";
  mFormatter.data()->undefState(str);
  setText(str);

  mDataReader = _dataSource->createReader( _dataName,
      [this](
        QSharedPointer<QByteArray> _data, 
        LERemoteDataStatus _status)
      {
        if(mFlagActive)
        {
          if(_status != LERemoteDataStatus::DS_OK)
          {
            QString str = "Undef";
            mFormatter.data()->undefState(str);
            setText(str);
            setEnabled(false);
            return;
          }
          QString str = mFormatter.data()->toString(*_data);
          setText(str);
          setEnabled(true);
        }
      }
      );
}

LCQRemLabel::~LCQRemLabel()
{
}

//------------------------------------------------------------------------------setActive
void LCQRemLabel::setActive(bool _flag)
{
  if(_flag)
  {
    mFlagActive = true;
    mDataReader->connectToSource();
  }
  else
  {
    mFlagActive = false;
    setEnabled(false);
    mDataReader->disconnectFromSource();
  }
}

//------------------------------------------------------------------------------event
bool LCQRemLabel::event(QEvent *_event)
{
  bool ret = false;
  switch(_event->type())
  {
  case QEvent::Type::Show:
    /* mDataReader->connectToSource(); */
    setActive(true);
    ret = true;
    break;
  case QEvent::Type::Hide:
    setActive(false);
    /* mDataReader->disconnectFromSource(); */
    ret = true;
    break;
  default:
    break;
  }
  QLabel::event(_event);
  return ret;
}


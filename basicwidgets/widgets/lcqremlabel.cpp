#include "lcqremlabel.h"
#include <QCoreApplication>
#include <QDebug>
#include <qnamespace.h>

//==============================================================================LCQRemLabel
LCQRemLabel::CReadListener::CReadListener(LCQRemLabel& _label) : 
  mLabel(_label),
  mFlagActive(false)
{

}

void LCQRemLabel::CReadListener::dataIsRead(QSharedPointer<QByteArray>  _data, 
    LERemoteDataStatus          _status)
{
  if(mFlagActive)
  {
    if(_status != LERemoteDataStatus::DS_OK)
    {
      QString str = "Undef";
      mLabel.mFormatter.data()->undefState(str);
      mLabel.setText(str);
      mLabel.setEnabled(false);
      return;
    }
    QString str = mLabel.mFormatter.data()->toString(*_data);
    mLabel.setText(str);
    mLabel.setEnabled(true);
  }
}

//==============================================================================LCQRemLabel
LCQRemLabel::LCQRemLabel(QWidget* _parent) : QLabel(_parent)
{
  setText("LCQRemLabel");
  setEnabled(false);
}

LCQRemLabel::LCQRemLabel(QString _text, QWidget* _parent) : QLabel(_text, _parent)
{
  setEnabled(false);
}

LCQRemLabel::LCQRemLabel(const QString& _dataName,
    QSharedPointer<LIRemoteDataSource> _dataSource,
    QSharedPointer<LIDataFormatter> _formatter,
    QWidget* _parent) :    QLabel(_parent),
  mDataName(_dataName),
  mFormatter(_formatter)
{
  setEnabled(false);
  QString str = "Undef";
  mFormatter.data()->undefState(str);
  setText(str);
  mDataListener = QSharedPointer<CReadListener>(new CReadListener(*this));
  mDataReader = _dataSource->createReader(_dataName, mDataListener);
}

LCQRemLabel::~LCQRemLabel()
{
}

//------------------------------------------------------------------------------setActive
void LCQRemLabel::setActive(bool _flag)
{
  if(_flag)
  {
    mDataListener->setActive(true);
    mDataReader->connectToSource();
  }
  else
  {
    mDataListener->setActive(false);
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


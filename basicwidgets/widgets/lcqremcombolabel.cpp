#include "lcqremcombolabel.h"
#include <QCoreApplication>
#include <QDebug>
#include <QMap>
#include "LIMovieAccess.h"
#include <QMovie>
#include <qnamespace.h>
#include <QLabel>

//==============================================================================SOwnData
//Собственные данные класса.
struct SOwnData
{
  //Карта соответствий прочитанных данных и выводимых строк.
  QWidget* undefItem; //Сущность для неопределенного состояния данных.
  QWidget* wrongItem; //Сущность для данных для которых нет соответствия.
  QMap<QString, QWidget*> normalItemMap;
  SOwnData(): undefItem(nullptr), wrongItem(nullptr){}
};

//==============================================================================LCQRemComboLabel
LCQRemComboLabel::
CReadListener::
CReadListener(LCQRemComboLabel& _label) : mLabel(_label), mFlagActive(false)
{

}

//------------------------------------------------------------------------------
#define L_LABEL_OWNDATA (static_cast<SOwnData*>(mLabel.mpOwnData))

//------------------------------------------------------------------------------dataIsRead
void LCQRemComboLabel::CReadListener::dataIsRead( 
    QSharedPointer<QByteArray> _data, LERemoteDataStatus _status)
{

  if(mFlagActive)
  {
    if(_status != LERemoteDataStatus::DS_OK)
    {
      if(L_LABEL_OWNDATA->undefItem != nullptr)
      {
        if(mLabel.currentWidget() != L_LABEL_OWNDATA->undefItem) 
          mLabel.setCurrentWidget(L_LABEL_OWNDATA->undefItem);
      }
      mLabel.setEnabled(false);
      return;
    }

    mLabel.setEnabled(true);

    auto it = L_LABEL_OWNDATA->normalItemMap.find( mLabel.mspFormatter->toString(*_data));

    if(it == L_LABEL_OWNDATA->normalItemMap.end())
    {
      if(L_LABEL_OWNDATA->wrongItem != nullptr)
      {
        if(mLabel.currentWidget() != L_LABEL_OWNDATA->wrongItem) 
          mLabel.setCurrentWidget(L_LABEL_OWNDATA->wrongItem);
      }
      return;
    }

    if(it.value() != mLabel.currentWidget())
    {
      mLabel.setCurrentWidget(it.value());
    }
  }
}

//==============================================================================LCQRemComboLabel
LCQRemComboLabel::LCQRemComboLabel(const QString& _dataName,
    QSharedPointer<LIRemoteDataSource> _dataSource,
    QSharedPointer<LIDataFormatter> _formatter,
    QWidget* _parent) :    QStackedWidget(_parent),
  mDataName(_dataName),
  mspFormatter(_formatter)
{
  mpOwnData = new SOwnData();

  QString str = "Undef";
  _formatter->undefState(str);
  addItemUndef(new QLabel(str));

  str = "Wrong";
  _formatter->wrongState(str);
  addItemWrong(new QLabel(str));

  mDataListener = QSharedPointer<CReadListener>(new CReadListener(*this));
  mDataReader = _dataSource->createReader(_dataName, mDataListener);

  setEnabled(false);
}

//------------------------------------------------------------------------------
#define L_OWNDATA (static_cast<SOwnData*>(mpOwnData))

//------------------------------------------------------------------------------~LCQRemComboLabel
LCQRemComboLabel::~LCQRemComboLabel()
{
  delete L_OWNDATA;
}

//------------------------------------------------------------------------------setActive
void LCQRemComboLabel::setActive(bool _flag)
{
  if(_flag)
  {
    mDataReader->connectToSource();
    mDataListener->setActive(true);
  }
  else
  {
    mDataReader->disconnectFromSource();
    setEnabled(false);
    mDataListener->setActive(false);
  }
}

//------------------------------------------------------------------------------addItem
void LCQRemComboLabel::addItem(QWidget* _widget, const QString&  _val)
{
  auto it = L_OWNDATA->normalItemMap.find(_val);

  if(it != L_OWNDATA->normalItemMap.end())
  {
    if(it.value() == _widget) return;
    removeWidget(it.value());
    L_OWNDATA->normalItemMap.erase(it);
    it.value()->deleteLater();
  }
  L_OWNDATA->normalItemMap.insert(_val, _widget);
  addWidget(_widget);
}

//------------------------------------------------------------------------------addItemUndef
void LCQRemComboLabel::addItemUndef(QWidget* _widget)
{
  if(L_OWNDATA->undefItem != nullptr)
  {
    removeWidget(L_OWNDATA->undefItem);
    L_OWNDATA->undefItem->deleteLater();
  }
  L_OWNDATA->undefItem = _widget;
  addWidget(_widget);
  setCurrentWidget(_widget);
}

//------------------------------------------------------------------------------addItemWrong
void LCQRemComboLabel::addItemWrong(QWidget* _widget)
{
  if(L_OWNDATA->wrongItem != nullptr)
  {
    removeWidget(L_OWNDATA->wrongItem);
    L_OWNDATA->wrongItem->deleteLater();
  }
  L_OWNDATA->wrongItem = _widget;
  addWidget(_widget);
}

//------------------------------------------------------------------------------event
bool LCQRemComboLabel::event(QEvent *_event)
{
  bool ret = false;
  switch(_event->type())
  {
  case QEvent::Type::Show:
    setActive(true);
    /* mDataReader->connectToSource(); */
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
  QStackedWidget::event(_event);
  return ret;
}


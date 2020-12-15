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
  QWidget* undefItem; //Сущность для неопределенного состояния данных.
  QWidget* wrongItem; //Сущность для данных для которых нет соответствия.
  //Карта соответствий прочитанных данных и выводимых строк.
  QMap<QString, QWidget*> normalItemMap;
  SOwnData(): undefItem(nullptr), wrongItem(nullptr){}
};


//------------------------------------------------------------------------------
#define L_OWNDATA (static_cast<SOwnData*>(mpOwnData))

//==============================================================================LCQRemComboLabel
LCQRemComboLabel::LCQRemComboLabel(const QString& _dataName,
    QSharedPointer<LIRemoteDataSource> _dataSource,
    QSharedPointer<LIDataFormatter> _formatter,
    QWidget* _parent) :    QStackedWidget(_parent),
  mspFormatter(_formatter)
{
  mpOwnData = new SOwnData();

  QString str = "Undef";
  _formatter->undefState(str);
  addItemUndef(new QLabel(str));

  str = "Wrong";
  _formatter->wrongState(str);
  addItemWrong(new QLabel(str));

  mDataReader = _dataSource->createReader(_dataName, 
      [this](QSharedPointer<QByteArray> _data, LERemoteDataStatus _status)
      {
        if(mFlagActive)
        {
          if(_status != LERemoteDataStatus::DS_OK)
          {
            if(L_OWNDATA->undefItem != nullptr)
            {
              if(currentWidget() != L_OWNDATA->undefItem) 
                setCurrentWidget(L_OWNDATA->undefItem);
            }
            setEnabled(false);
            return;
          }
          setEnabled(true);
          auto it = L_OWNDATA->normalItemMap.find( mspFormatter->toString(*_data));
          if(it == L_OWNDATA->normalItemMap.end())
          {
            if(L_OWNDATA->wrongItem != nullptr)
            {
              if(currentWidget() != L_OWNDATA->wrongItem) 
                setCurrentWidget(L_OWNDATA->wrongItem);
            }
            return;
          }
          if(it.value() != currentWidget())
          {
            setCurrentWidget(it.value());
          }
        }
      });

  setEnabled(false);
}


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
    mFlagActive = true;
  }
  else
  {
    mDataReader->disconnectFromSource();
    setEnabled(false);
    mFlagActive = false;
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
    ret = true;
    break;
  case QEvent::Type::Hide:
    setActive(false);
    ret = true;
    break;
  default:
    break;
  }
  QStackedWidget::event(_event);
  return ret;
}


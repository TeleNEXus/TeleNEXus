#include "lcqremcombobox.h"
#include "LIRemoteDataReader.h"
#include "LIRemoteDataWriter.h"
#include <QCoreApplication>
#include <QDebug>
#include <qcombobox.h>
#include <qglobal.h>
#include <qnamespace.h>
#include <QKeyEvent>
//==============================================================================CReadListener
LCQRemComboBox::
CReadListener::
CReadListener(LCQRemComboBox& _combobox) :  mOwner(_combobox)
{
}

//------------------------------------------------------------------------------
void LCQRemComboBox::CReadListener::dataIsRead( 
    QSharedPointer<QByteArray>  _data, 
    LERemoteDataStatus          _status)
{
  switch(_status)
  {
  case LERemoteDataStatus::DS_OK:
    mOwner.setCurrentIndex(
        mOwner.findData( 
          mOwner.mFormatter->
          toString( *_data.data())) ); 
    mOwner.setEnabled(true);
    break;

  case LERemoteDataStatus::DS_WRONG:
    mOwner.setCurrentIndex(-1);
    mOwner.setEnabled(true);
    break;

  case LERemoteDataStatus::DS_UNDEF:
    mOwner.setCurrentIndex(-1);
    mOwner.setEnabled(false);
  default:
    break;
  }
}

//==============================================================================LCQRemComboBox
//LCQRemComboBox::LCQRemComboBox(QWidget* _parent) : QComboBox(_parent)
//{
//}

//------------------------------------------------------------------------------
LCQRemComboBox::LCQRemComboBox( 
    const QString&                              _dataNameRead,
    const QString&                              _dataNameWrite,
    QSharedPointer<LIRemoteDataSource>          _dataSource,
    QSharedPointer<LIDataFormatter>             _formatter,
    QWidget* _parent):  QComboBox(_parent),
  mFormatter(_formatter),
  mFlagPopupOn(false)
{

  mDataReadListener = QSharedPointer<CReadListener>(new CReadListener(*this));

  mDataReader = _dataSource->createReader(_dataNameRead, mDataReadListener);

  mDataWriter = _dataSource->createWriter(_dataNameWrite, 
      QSharedPointer<LIRemoteDataWriteListener>(nullptr));

  this->setEnabled(false);

  connect(this, 
      static_cast <void(LCQRemComboBox::*)(int)> 
      (&LCQRemComboBox::activated),
      [&](int index)
      {
      Q_UNUSED(index);
      mDataWriter->writeRequest(
          mFormatter->toBytes(currentData().toString()));
      });
}

//------------------------------------------------------------------------------
LCQRemComboBox::~LCQRemComboBox()
{
}

//------------------------------------------------------------------------------
bool LCQRemComboBox::event(QEvent *_event)
{
  int key;
  switch(_event->type())
  {
  case QEvent::Type::Show:
    mDataReader->connectToSource();
    setCurrentIndex(-1);
    return false;

  case QEvent::Type::Hide:
    mDataReader->disconnectFromSource();
    return false;

  case QEvent::Type::KeyPress:
    //Очиска фокуса видета при нажатии клавиши Escape.
    key = static_cast<QKeyEvent*>(_event)->key();
    if(( key == Qt::Key_Enter) || ( key == Qt::Key_Return))
    {
      showPopup();
    } else if( static_cast<QKeyEvent*>(_event)->key() == Qt::Key_Escape)
    {
      if ( mFlagPopupOn )
      {
        hidePopup();
      }
      else
      {
        clearFocus();
      }
    }
    return false;

  default:
    break;
  }
  return QComboBox::event(_event);
}

//------------------------------------------------------------------------------
void LCQRemComboBox::showPopup(void)
{
  mFlagPopupOn = true;
  QComboBox::showPopup();
}

//------------------------------------------------------------------------------
void LCQRemComboBox::hidePopup(void)
{
  mFlagPopupOn = false;
  QComboBox::hidePopup();
}

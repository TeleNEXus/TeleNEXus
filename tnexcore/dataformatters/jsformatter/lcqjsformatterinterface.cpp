#include "lcqjsformatterinterface.h"
#include <QSharedPointer>
#include <QDebug>


//==============================================================================LCQJSFormatterInterface
LCQJSFormatterInterface::LCQJSFormatterInterface()
{
}

//------------------------------------------------------------------------------
LCQJSFormatterInterface::~LCQJSFormatterInterface()
{
}

//==============================================================================
static void laterDeleter(QObject* _obj)
{
  _obj->deleteLater();
}

//------------------------------------------------------------------------------
QSharedPointer<LCQJSFormatterInterface> LCQJSFormatterInterface::create()
{
  static QWeakPointer<LCQJSFormatterInterface> wp_instance;
  auto sp = wp_instance.lock();
  if(sp.isNull())
  {
    sp = QSharedPointer<LCQJSFormatterInterface>(
        new LCQJSFormatterInterface(),
        laterDeleter);
  }
  return sp;
}

//------------------------------------------------------------------------------
void LCQJSFormatterInterface::debugOut(const QString& _str)
{
  qDebug("%s", qPrintable(_str));
}

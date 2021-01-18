#include "lcqjsformatterinterface.h"
#include <QSharedPointer>
#include <QDebug>


//==============================================================================laterDeleter
static void laterDeleter(QObject* _obj)
{
  _obj->deleteLater();
}

//==============================================================================LCQJSFormatterInterface
LCQJSFormatterInterface::LCQJSFormatterInterface()
{
}

//------------------------------------------------------------------------------
LCQJSFormatterInterface::~LCQJSFormatterInterface()
{
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

//------------------------------------------------------------------------------
void LCQJSFormatterInterface::exportModule(const QString& _moduleFileName)
{
}

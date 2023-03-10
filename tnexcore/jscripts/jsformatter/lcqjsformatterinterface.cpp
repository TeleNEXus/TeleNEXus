#include "lcqjsformatterinterface.h"
#include "jscriptcommon.h"
#include "applicationinterface.h"
#include <QSharedPointer>
#include <QJSEngine>
#include <QJSValueIterator>
#include <QDebug>


//==============================================================================laterDeleter
static void laterDeleter(QObject* _obj)
{
  _obj->deleteLater();
}

//==============================================================================LCQJSFormatterInterface
LCQJSFormatterInterface::LCQJSFormatterInterface(QJSEngine* _engine) :
  mpEngine(_engine)
{
}

//------------------------------------------------------------------------------
LCQJSFormatterInterface::~LCQJSFormatterInterface()
{
}


//------------------------------------------------------------------------------
QSharedPointer<LCQJSFormatterInterface> 
LCQJSFormatterInterface::create(QJSEngine* _engine)
{
  return QSharedPointer<LCQJSFormatterInterface>(
        new LCQJSFormatterInterface(_engine),
        laterDeleter);
}

//------------------------------------------------------------------------------
void LCQJSFormatterInterface::message(const QString& _str)
{
  CApplicationInterface::getInstance().message(_str);
}
//------------------------------------------------------------------------------
void LCQJSFormatterInterface::warning(const QString& _str)
{
  CApplicationInterface::getInstance().warning(_str);
}
//------------------------------------------------------------------------------
void LCQJSFormatterInterface::error(const QString& _str)
{
  CApplicationInterface::getInstance().error(_str);
}
//------------------------------------------------------------------------------
void LCQJSFormatterInterface::importModule(
    const QString& _fileName,
    const QString& _propertyName)
{
  jscriptcommon::importModule(*mpEngine, _fileName, _propertyName);
}

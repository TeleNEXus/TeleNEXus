#include "lcqjsformatterinterface.h"
#include <QDebug>


//==============================================================================LCQJSFormatterInterface
LCQJSFormatterInterface::LCQJSFormatterInterface()
{
}

//------------------------------------------------------------------------------
LCQJSFormatterInterface::~LCQJSFormatterInterface()
{
}

//------------------------------------------------------------------------------
void LCQJSFormatterInterface::setProperty(
    const QString& _propName, QJSEngine& _jsengine)
{

  static LCQJSFormatterInterface instance;

  QJSValue jsappinterface = _jsengine.newQObject(&instance);
  _jsengine.globalObject().setProperty(_propName, jsappinterface);
}

//------------------------------------------------------------------------------
void LCQJSFormatterInterface::debugOut(const QString& _str)
{
  qDebug() << _str;
}

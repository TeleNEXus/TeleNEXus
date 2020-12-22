#include "jsvalidator.h"
#include "jsclassfunc.h"
#include <QDebug>

LCQJsValidator::LCQJsValidator(const QJSValue& _callObject, QObject* _parent) :
  QValidator(_parent),
  mCallObject(_callObject)
{
}

LCQJsValidator::State LCQJsValidator::validate(QString& _input, int& _pos) const
{

  Q_UNUSED(_pos);
  State state = State::Intermediate;

  /* QJSValue call = mCallObject; */

  QJSValue jsret = mCallObject.call(QJSValueList() << _input);

  if(jsret.isError())
  {
    qDebug() 
      << "Uncaught exception at line"
      << jsret.property("lineNumber").toInt()
      << ":" << jsret.toString();
    return State::Intermediate;
  }

  if(jsret.isNumber()) qDebug() << "JS Validatro Return is number.";

  qDebug() << "JS Validator Return = " << jsret.toVariant().toInt();

  switch(*static_cast<const EJSValidateState*>(jsret.toVariant().constData()))
  {
  case EJSValidateState::acceptable:
    qDebug() << "JS Validator Return = " << "Acceptable";
    state = State::Acceptable;
    break;

  case EJSValidateState::intermediate:
    qDebug() << "JS Validator Return = " << "Intermediate";
    state = State::Intermediate;
    break;

  case EJSValidateState::invalid:
    qDebug() << "JS Validator Return = " << "Invalid";
    state = State::Invalid;
    break;
  }

  return state;
}

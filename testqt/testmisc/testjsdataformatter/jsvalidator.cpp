#include "jsvalidator.h"
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
  state = static_cast<State>(jsret.toInt());

  switch(state)
  {
  case State::Acceptable:
    qDebug() << "JS Validator Return = " << "Acceptable";
    state = State::Acceptable;
    break;

  case State::Intermediate:
    qDebug() << "JS Validator Return = " << "Intermediate";
    state = State::Intermediate;
    break;

  case State::Invalid:
    qDebug() << "JS Validator Return = " << "Invalid";
    state = State::Invalid;
    break;
  default:
    state = State::Intermediate;
    break;
  }

  return state;
}

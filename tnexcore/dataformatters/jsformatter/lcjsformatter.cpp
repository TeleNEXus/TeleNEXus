#include "lcjsformatter.h"
#include <QValidator>
#include <QDomElement>
#include <QStringList>
#include <QFile>
#include <QJSValue>
#include <QDebug>

//==============================================================================
const QString __slFormatterInterfacePropName = "FORMATTEREXTERN";
const QString __slFormatterAttributesPropName = "Attributes";

//==============================================================================
static void emitError(const QJSValue& _value);
static QString createScriptHeader(const QDomNamedNodeMap& _attributes);

//==============================================================================__slAttributes
static const struct 
{
  QString jsfile = "jsfile";
}__slAttributes;

//==============================================================================
class CJSValidator : public QValidator
{
private:
  mutable QJSValue mCallObject;

public:
  CJSValidator() = delete;
  CJSValidator(const QJSValue& _callObject) : 
    QValidator(nullptr),
    mCallObject(_callObject)
  {
  }

  //----------------------------------------------------------------------------
  virtual State validate(QString& _input, int& _pos) const override
  {
    Q_UNUSED(_pos);
    QJSValue jsret = mCallObject.call(QJSValueList() << _input);

    if(jsret.isError())
    {
      emitError(jsret);
      return State::Intermediate;
    }

    if(!jsret.isNumber()) return State::Intermediate;

    switch(static_cast<State>(jsret.toInt()))
    {
    case State::Acceptable:
      /* qDebug() << "JS Validator Return = " << "Acceptable"; */
      return State::Acceptable;

    case State::Intermediate:
      /* qDebug() << "JS Validator Return = " << "Intermediate"; */
      return State::Intermediate;

    case State::Invalid:
      /* qDebug() << "JS Validator Return = " << "Invalid"; */
      return State::Invalid;

    default:
      return State::Intermediate;
    }
  }
};

//==============================================================================LCJSFormatter
LCJSFormatter::LCJSFormatter(const QDomElement& _element)
{
  QString attr = _element.attribute(__slAttributes.jsfile);
  if(attr.isNull()) return;

  /* QFile scriptFile(attr); */
  /* QString script; */

  /* if (!scriptFile.open(QIODevice::ReadOnly)) return nullptr; */

  /* QTextStream stream(&scriptFile); */
  /* script = stream.readAll(); */
  /* scriptFile.close(); */
}

//------------------------------------------------------------------------------
LCJSFormatter::~LCJSFormatter()
{
  if(mpValidator)
  {
    mpValidator->deleteLater();
  }
}

//------------------------------------------------------------------------------
QString LCJSFormatter::toString(const QByteArray& _data)
{
  Q_UNUSED(_data);
  return QString();
}

//------------------------------------------------------------------------------
QByteArray LCJSFormatter::toBytes(const QString& _str)
{
  Q_UNUSED(_str);
  return QByteArray();
}

//------------------------------------------------------------------------------
QString LCJSFormatter::fitting(const QString& _str)
{
  Q_UNUSED(_str);
  return QString();
}

//------------------------------------------------------------------------------
QValidator* LCJSFormatter::validator()
{
  return mpValidator;
}

//------------------------------------------------------------------------------
QSharedPointer<LCJSFormatter> LCJSFormatter::create(const QDomElement& _element)
{
  return QSharedPointer<LCJSFormatter>(new LCJSFormatter(_element));
}

//==============================================================================createScriptHeader
static QString createScriptHeader(const QDomNamedNodeMap& _attributes)
{

  QString obj_attributes = 
    QString("var %1 = { \n").arg(__slFormatterAttributesPropName);
  for(int i = 0; i < _attributes.length(); i++)
  {
    auto node  = _attributes.item(i);
    obj_attributes += QString("%1 : %2, \n")
      .arg(node.nodeName())
      .arg(node.nodeValue());
  }

  obj_attributes += "};\n";

  return QString(
      "var Acceptable   = %1;"
      "var Intermediate = %2;"
      "var Invalid      = %3;"
      "%4;"
      "var DebugOut = %5.debugOut;"
      )
    .arg(QValidator::State::Acceptable)
    .arg(QValidator::State::Intermediate)
    .arg(QValidator::State::Invalid)
    .arg(obj_attributes)
    .arg(__slFormatterInterfacePropName)
    ;
}

//==============================================================================emitError
static void emitError(const QJSValue& _value)
{
  qDebug() 
    << "Uncaught exception at line"
    << _value.property("lineNumber").toInt()
    << ":" << _value.toString();
}

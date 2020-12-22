#include "lcjsformatter.h"
#include <QValidator>
#include <QDomElement>
#include <QStringList>
#include <QFile>
#include <QJSValue>
#include <QJSEngine>
#include <QDebug>

//==============================================================================
static const struct
{
  QString formatterInterface  = "FORMATTEREXTERN";
  QString attributes          = "Attributes";
  QString funcValidate        = "Validate";
  QString funcToBytes         = "ToBytes";
  QString funcToString        = "ToString";
  QString funcFitting         = "Fitting"; 
}__slPropNames;

//==============================================================================__slAttributes
static const struct 
{
  QString jsfile = "jsfile";
}__slAttributes;

//==============================================================================
static void emitError(const QJSValue& _value);
static QString createScriptHeader(const QDomNamedNodeMap& _attributes);


//==============================================================================
class CJSValidator : public QValidator
{
private:
  mutable QJSValue mCallValue;

public:
  CJSValidator() : 
    QValidator(nullptr)
  {
  }

  void setCallValue(const QJSValue& _value)
  {
    mCallValue = _value;
  }

  //----------------------------------------------------------------------------
  virtual State validate(QString& _input, int& _pos) const override
  {
    Q_UNUSED(_pos);
    QJSValue jsret = mCallValue.call(QJSValueList() << _input);

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

//==============================================================================SLocalData
struct SLocalData
{
  QJSEngine jsengine;
  CJSValidator* validator;
  QJSValue callToString;
  QJSValue callToBytes;
  QJSValue callFitting;
  SLocalData() :
    validator(new CJSValidator())
  {
  }
  ~SLocalData()
  {
    validator->deleteLater();
  }
};

//==============================================================================
#define mpLocalData (static_cast<SLocalData*>(mpData))

//==============================================================================LCJSFormatter
LCJSFormatter::LCJSFormatter(const QDomElement& _element)
{
  mpData = new SLocalData;
  QString attr = _element.attribute(__slAttributes.jsfile);

  QFile file(attr);

  if (!file.open(QIODevice::ReadOnly)) 
  {
    qDebug() << "LCJSFormatter can't open file " << file.fileName();
    return;
  }

  QString script;
  QTextStream stream(&file);
  script = stream.readAll();
  file.close();

  QJSValue jsvalue = mpLocalData->jsengine.evaluate(
      QString("%1 \n %2")
      .arg(createScriptHeader(_element.attributes()))
      .arg(script));

  if(jsvalue.isError()) { emitError(jsvalue); }

  mpLocalData->validator->setCallValue(
      mpLocalData->jsengine.globalObject().property(
        __slPropNames.funcValidate));

  mpLocalData->callToString = 
    mpLocalData->jsengine.globalObject().property( __slPropNames.funcToString);

  mpLocalData->callToBytes = 
    mpLocalData->jsengine.globalObject().property( __slPropNames.funcToBytes);

  mpLocalData->callFitting = 
    mpLocalData->jsengine.globalObject().property( __slPropNames.funcFitting);
}

//------------------------------------------------------------------------------
LCJSFormatter::~LCJSFormatter()
{
  delete mpLocalData;
}

//------------------------------------------------------------------------------
QString LCJSFormatter::toString(const QByteArray& _data)
{
  QJSValue jsarray = mpLocalData->jsengine.newArray(_data.size());
  for(int i = 0; i < _data.size(); i++)
  {
    jsarray.setProperty(i, _data[i]);
  }
  QJSValue jsret = mpLocalData->callToString.call(QJSValueList() << jsarray);
  if(jsret.isString()) return jsret.toString();
  return QString();
}

//------------------------------------------------------------------------------
QByteArray LCJSFormatter::toBytes(const QString& _str)
{
  QByteArray ret_data;

  QJSValue jsret = mpLocalData->callToBytes.call(QJSValueList() << _str);

  if(!jsret.isArray()) return ret_data;
  if(!jsret.property("length").isNumber()) return ret_data;

  int array_size = jsret.property("length").toUInt();

  if(array_size <= 0) return ret_data;

  ret_data.resize(array_size);

  for(int i = 0; i <  array_size; i++)
  {
    QJSValue val = jsret.property(i);
    if(!val.isNumber()) return QByteArray();
    ret_data[i] = val.toUInt();
  }
  return ret_data;
}

//------------------------------------------------------------------------------
QString LCJSFormatter::fitting(const QString& _str)
{
  QJSValue jsret = mpLocalData->callFitting.call(QJSValueList() << _str);
  if(!jsret.isString()) return QString();
  return jsret.toString();
}

//------------------------------------------------------------------------------
QValidator* LCJSFormatter::validator()
{
  return mpLocalData->validator;
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
    QString("var %1 = { \n").arg(__slPropNames.attributes);
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
    .arg(__slPropNames.formatterInterface)
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

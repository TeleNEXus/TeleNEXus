#include "lcjsformatter.h"
#include "LIApplication.h"
#include "lcqjsformatterinterface.h"

#include <QValidator>
#include <QDomElement>
#include <QStringList>
#include <QFile>
#include <QJSValue>
#include <QJSEngine>
#include <QSharedPointer>
#include <QDebug>

//==============================================================================__slPropNames
static const struct
{
  QString formatterInterface  = "FORMATTERINTERFACEEXPORT";
  QString globalExport        = "GLOBALEXPORT";
  QString attributes          = "Attributes";
  QString funcValidate        = "Validate";
  QString funcToBytes         = "ToBytes";
  QString funcToString        = "ToString";
  QString funcFitting         = "Fitting"; 
}__slPropNames;

//==============================================================================__slAttributes
static const struct 
{
  QString file = "file";
}__slAttributes;

//==============================================================================
static void emitError(const QJSValue& _value);
static QString createScriptHeader(const QDomNamedNodeMap& _attributes);


//==============================================================================CJSValidator
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

    State state = static_cast<State>(jsret.toInt());

    if(!jsret.isNumber()) return State::Intermediate;

    switch(state)
    {
    case State::Acceptable:
      qDebug() << "JS Validator Return = " << "Acceptable";
      break;

    case State::Intermediate:
      qDebug() << "JS Validator Return = " << "Intermediate";
      break;

    case State::Invalid:
      qDebug() << "JS Validator Return = " << "Invalid";
      break;

    default:
      return State::Intermediate;
    }
    return state;
  }
};

//==============================================================================SLocalData
struct SLocalData
{
  CJSValidator* validator;
  QSharedPointer<LCQJSFormatterInterface> formatterInterface;
  QJSEngine *jsengine;
  QJSValue callToString;
  QJSValue callToBytes;
  QJSValue callFitting;
  SLocalData() = delete;
  SLocalData(QSharedPointer<LCQJSFormatterInterface> _formatterInterface) :
    validator(new CJSValidator()),
    formatterInterface(_formatterInterface),
    jsengine(new QJSEngine())
  {
  }
  ~SLocalData()
  {
    validator->deleteLater();
    jsengine->deleteLater();
  }
};

//==============================================================================mpLocalData
#define mpLocalData (static_cast<SLocalData*>(mpData))

//==============================================================================LCJSFormatter
LCJSFormatter::LCJSFormatter(const QDomElement& _element,
    const QString& _appPath):
  mpData(new SLocalData(LCQJSFormatterInterface::create()))
{

  QString file_name = _element.attribute(__slAttributes.file);

  if(file_name.isNull()) return;

  QFile file(QString("%1%2").arg(_appPath).arg(file_name));

  if (!file.open(QIODevice::ReadOnly)) 
  {
    qDebug() << "LCJSFormatter can't open file " << file.fileName();
    return;
  }
  qDebug() << "LCJSFormatter open JavaScript file " << file.fileName();

  QTextStream stream(&file);
  QString script = stream.readAll();
  file.close();

  qDebug() << "Script =======================================";
  auto fi = mpLocalData->formatterInterface;

  QJSValue jsvalue = 
    mpLocalData->jsengine->newQObject(
        mpLocalData->formatterInterface.data());

  mpLocalData->jsengine->globalObject().setProperty(
      __slPropNames.formatterInterface, jsvalue);

  jsvalue = 
    mpLocalData->jsengine->evaluate(
        createScriptHeader(_element.attributes()));

  mpLocalData->jsengine->globalObject().setProperty(
      __slPropNames.globalExport, jsvalue);

  jsvalue= mpLocalData->jsengine->evaluate(script);

  qDebug() << "Evaluate =======================================";
  if(jsvalue.isError()) { emitError(jsvalue); }

  mpLocalData->validator->setCallValue(
      mpLocalData->jsengine->globalObject().property(
        __slPropNames.funcValidate));

  mpLocalData->callToString = 
    mpLocalData->jsengine->globalObject().property( __slPropNames.funcToString);

  mpLocalData->callToBytes = 
    mpLocalData->jsengine->globalObject().property( __slPropNames.funcToBytes);

  mpLocalData->callFitting = 
    mpLocalData->jsengine->globalObject().property( __slPropNames.funcFitting);
}

//------------------------------------------------------------------------------
LCJSFormatter::~LCJSFormatter()
{
  delete mpLocalData;
}

//------------------------------------------------------------------------------
QString LCJSFormatter::toString(const QByteArray& _data)
{
  QJSValue jsarray = mpLocalData->jsengine->newArray(_data.size());
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
  /* if(!jsret.property("length").isNumber()) return ret_data; */

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
QSharedPointer<LCJSFormatter> LCJSFormatter::create(
    const QDomElement& _element,
    const LIApplication& _app)
{
  return QSharedPointer<LCJSFormatter>(
      new LCJSFormatter(_element, _app.getProjectPath()));
}

//==============================================================================createScriptHeader
static QString createScriptHeader(const QDomNamedNodeMap& _attributes)
{

  QString obj_attributes = 
    QString("var %1 = { ").arg(__slPropNames.attributes);
  for(int i = 0; i < _attributes.length(); i++)
  {
    auto node  = _attributes.item(i);
    obj_attributes += QString("%1 : '%2', ")
      .arg(node.nodeName())
      .arg(node.nodeValue());
  }
  obj_attributes += "};";

  QString out = QString(
      "%1"
      "var Acceptable   = %2;"
      "var Intermediate = %3;"
      "var Invalid      = %4;"
      "function DebugOut(str) {%5.debugOut(str)};"
      )
    .arg(obj_attributes)
    .arg(QValidator::State::Acceptable)
    .arg(QValidator::State::Intermediate)
    .arg(QValidator::State::Invalid)
    .arg(__slPropNames.formatterInterface)
    ;
  return out;
}

//==============================================================================emitError
static void emitError(const QJSValue& _value)
{
  qDebug() 
    << "Uncaught exception at line"
    << _value.property("lineNumber").toInt()
    << ":" << _value.toString();
}

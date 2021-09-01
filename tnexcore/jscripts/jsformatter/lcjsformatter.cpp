#include "lcjsformatter.h"
#include "LIApplication.h"
#include "lcqjsformatterinterface.h"
#include "jscriptcommon.h"

#include <QValidator>
#include <QDomElement>
#include <QStringList>
#include <QFile>
#include <QJSValue>
#include <QJSEngine>
#include <QSharedPointer>
#include <QFile>
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
}__slPropNames;

//==============================================================================createScriptGlobal
static QString createScriptGlobal(const QMap<QString, QString>& _attributes);


//==============================================================================CJSValidator
struct SLocalData;
class CJSValidator : public QValidator
{
private:
  const SLocalData& mLocalData;
  mutable QJSValue mCallValue;

public:
  CJSValidator(const SLocalData& _localData) : 
    QValidator(nullptr),
    mLocalData(_localData)
  {
  }

  void setCallValue(const QJSValue& _value)
  {
    mCallValue = _value;
  }

  //----------------------------------------------------------------------------
  virtual State validate(QString& _input, int& _pos) const override;
};

//==============================================================================SLocalData
struct SLocalData
{
  QString scriptFileName;
  QJSEngine *jsengine;
  CJSValidator* validator;
  QSharedPointer<LCQJSFormatterInterface> formatterInterface;
  QJSValue callToString;
  QJSValue callToBytes;
  QJSValue callFitting;
  SLocalData() = delete;
  SLocalData(const QString& _scriptFileName) :
    scriptFileName(_scriptFileName),
    jsengine(new QJSEngine()),
    validator(new CJSValidator(*this)),
    formatterInterface(LCQJSFormatterInterface::create(jsengine))
  {
    jsengine->installExtensions(QJSEngine::Extension::AllExtensions);
  }
  ~SLocalData()
  {
    validator->deleteLater();
    jsengine->deleteLater();
  }
};

//----------------------------------------------------------------------------CJSValidator::validate
CJSValidator::State CJSValidator::validate(QString& _input, int& _pos) const
{
  Q_UNUSED(_pos);
  QJSValue jsret = mCallValue.call(QJSValueList() << _input);

  if(jsret.isError())
  {
    jscriptcommon::emitRuntimeError(jsret, mLocalData.scriptFileName);
    return State::Intermediate;
  }

  State state = static_cast<State>(jsret.toInt());

  if(!jsret.isNumber()) return State::Intermediate;

  return state;
}

//==============================================================================mpLocalData
#define mpLocalData(p) (static_cast<SLocalData*>(p))
#define ld (*(static_cast<SLocalData*>(mpData)))

//==============================================================================LCJSFormatter
LCJSFormatter::LCJSFormatter( 
    const QString& _scriptFileName,
    const QString& _script,
    const QMap<QString, QString>& _attributes):
  mpData(new SLocalData(_scriptFileName))
{

  QJSValue jsvalue = 
    ld.jsengine->newQObject(
        ld.formatterInterface.data());

  ld.jsengine->globalObject().setProperty(
      __slPropNames.formatterInterface, jsvalue);


  ld.jsengine->evaluate(
      createScriptGlobal(_attributes));

  jsvalue= ld.jsengine->evaluate(_script, _scriptFileName);

  if(jsvalue.isError()) { jscriptcommon::emitEvaluateError(jsvalue); }

  ld.validator->setCallValue(
      ld.jsengine->globalObject().property(
        __slPropNames.funcValidate));

  ld.callToString = 
    ld.jsengine->globalObject().property( __slPropNames.funcToString);

  ld.callToBytes = 
    ld.jsengine->globalObject().property( __slPropNames.funcToBytes);
}

//------------------------------------------------------------------------------
LCJSFormatter::~LCJSFormatter()
{
  delete mpLocalData(mpData);
}

//------------------------------------------------------------------------------
QString LCJSFormatter::toString(const QByteArray& _data)
{
  QJSValue jsarray = ld.jsengine->newArray(_data.size());
  for(int i = 0; i < _data.size(); i++)
  {
    jsarray.setProperty(i, _data[i]);
  }

  QJSValue jsret = ld.callToString.call(QJSValueList() << jsarray);

  if(jsret.isError()) 
  {
    jscriptcommon::emitRuntimeError(jsret, ld.scriptFileName);
    return QString();
  }

  if(jsret.isString()) return jsret.toString();

  return QString();
}

//------------------------------------------------------------------------------
QByteArray LCJSFormatter::toBytes(const QString& _str)
{
  QByteArray ret_data;

  QJSValue jsret = ld.callToBytes.call(QJSValueList() << _str);
  if(jsret.isError()) 
  {
    jscriptcommon::emitRuntimeError(jsret, ld.scriptFileName);
    return ret_data;
  }

  if(!jsret.isArray()) return ret_data;

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
QValidator* LCJSFormatter::validator()
{
  return ld.validator;
}

//------------------------------------------------------------------------------
QSharedPointer<LCJSFormatter> LCJSFormatter::create(
    const QString& _fileName,
    const QMap<QString, QString>& _attributes)
{
  auto ret_wrong = [](){ return QSharedPointer<LCJSFormatter>(); };

  QFile file(_fileName);
  if (!file.open(QIODevice::ReadOnly)) return ret_wrong();
  QString script(file.readAll());
  file.close();

  if(script.isNull()) return ret_wrong();

  return QSharedPointer<LCJSFormatter>(
      new LCJSFormatter(_fileName, script,  _attributes));
}

//==============================================================================createScriptGlobal
static QString createScriptGlobal(const QMap<QString, QString>& _attributes)
{
  QString obj_attributes = 
    QString("var %1 = { ").arg(__slPropNames.attributes);

  for(auto it = _attributes.begin(); it != _attributes.end(); it++)
  {
    obj_attributes += QString("%1 : '%2', ")
      .arg(it.key())
      .arg(it.value());
  }

  obj_attributes += "};";

  QString out = QString(
      "%1"
      "var Acceptable   = %2;"
      "var Intermediate = %3;"
      "var Invalid      = %4;"
      "function Message(str) {%5.message(str)};"
      "function Warning(str) {%5.warning(str)};"
      "function Error(str) {%5.error(str)};"
      "function ImportModule(_fileName, _property) {"
      "%5.importModule(_fileName, _property)};"
      )
/*1*/ .arg(obj_attributes)
/*2*/ .arg(QValidator::State::Acceptable)
/*3*/ .arg(QValidator::State::Intermediate)
/*4*/ .arg(QValidator::State::Invalid)
/*5*/ .arg(__slPropNames.formatterInterface);
  return out;
}


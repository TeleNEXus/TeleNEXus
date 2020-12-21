#include <QApplication>
#include <QtWidgets>
#include <qboxlayout.h>
#include <qglobal.h>
#include <qjsvalue.h>
#include <qnamespace.h>
#include <QScrollArea>
#include <qscrollarea.h>
#include <QComboBox>

#include <QList>
#include <QJSEngine>
#include "jsclassfunc.h"
#include "jsvalidator.h"

int main(int argc, char** argv)
{
  QApplication app(argc, argv);
  QFrame* widget = new QFrame;
  QVBoxLayout* layout = new QVBoxLayout;
  QLabel* label1 = new QLabel("Label 1");
  QLineEdit* line_edit1 = new QLineEdit("0");

  QString script_file_name = "../testjsfunc.js";
  QFile scriptFile(script_file_name);
  QString script;

  if (!scriptFile.open(QIODevice::ReadOnly)) qDebug() << "Catn't open file";
  else
  {
    qDebug() << "Script file " << script_file_name << " is opened";
    QTextStream stream(&scriptFile);
    script = stream.readAll();
    scriptFile.close();
  }

  LJsClass *appinterface = new LJsClass;

  /* LQValidateStatus* validate_status = new LQValidateStatus(); */

  QString propIntermediate  = "intermediate";
  QString propAcceptable    = "acceptable";
  QString propInvalid       = "invalid";

  QJSEngine jsengin;
  QJSValue jsappinterface = jsengin.newQObject(appinterface);
  /* QJSValue jsvalidatestatus = jsengin.newQObject(validate_status); */
  jsengin.globalObject().setProperty("Application", jsappinterface);
  /* jsengin.globalObject().setProperty("EValidateStatus", jsvalidatestatus); */
  /* QVariantMap attributes; */
  /* attributes["min"] = QVariant(10); */
  /* attributes["max"] = QVariant(100); */

  /* jsengin.globalObject().setProperty(propIntermediate, propIntermediate); */
  /* jsengin.globalObject().setProperty(propAcceptable, propAcceptable); */
  /* jsengin.globalObject().setProperty(propInvalid, propInvalid); */
  /* jsengin.globalObject().setProperty("ValidateStatus", ); */

  script = QString(
      "var attributes = { valmin: %1, valmax: %2}; \n %3")
    .arg(-100000)
    .arg(100000)
    .arg(script) ;

  QJSValue jsobject = jsengin.evaluate(script);

  if(jsobject.isError())
  {
    qDebug() 
      << "Uncaught exception at line"
      << jsobject.property("lineNumber").toInt()
      << ":" << jsobject.toString();
  }

  QJSValue jsvalidate = jsengin.globalObject().property("validate");
  LCQJsValidator jsvalidator(jsvalidate);

  line_edit1->setValidator(&jsvalidator);


  QObject::connect(line_edit1, &QLineEdit::textChanged, 
      [label1, &jsvalidate, &jsvalidator](const QString& _text)
      {
        label1->setText(_text);
        /* QJSValue jsret = jsvalidate.call(QJSValueList() << _text); */

        /* if(jsret.isError()) */
        /* { */
        /*   qDebug() */ 
        /*     << "Uncaught exception at line" */
        /*     << jsret.property("lineNumber").toInt() */
        /*     << ":" << jsret.toString(); */
        /* } */
        QString text = _text;
        int pos = 0;
        /* jsvalidator.validate(text, pos); */
      });

  layout->addWidget(label1);
  layout->addWidget(line_edit1);
  widget->setLayout(layout);

  widget->show();
  return app.exec();
}

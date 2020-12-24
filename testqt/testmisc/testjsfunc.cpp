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

  /* QString script_file_name = "../testjsfunc.js"; */
  QString script_file_name = "../testjsdataformatter/formatter.js";
  QFile scriptFile(script_file_name);
  QString script;

  if (!scriptFile.open(QIODevice::ReadOnly)) qDebug() << "Catn't open file";
  else
  {
    qDebug() << "Script file " << script_file_name << " is opened";
    qDebug() << "Script file " << scriptFile.fileName() << " is opened";
    QTextStream stream(&scriptFile);
    script = stream.readAll();
    scriptFile.close();
  }

  LJsClass *appinterface = new LJsClass;
 
  QJSEngine jsengin;
  QJSValue jsappinterface = jsengin.newQObject(appinterface);
  jsengin.globalObject().setProperty("APPLICATIONEXPORT", jsappinterface);

  QJSValue jsobject = jsengin.evaluate(QString( 
        "var Attributes = { "
        "valmin: %1,"
        "valmax: %2};" 
        "var Intermediate = %3;"
        "var Acceptable   = %4;"
        "var Invalid      = %5;"
        "function DebugOut(str) {APPLICATIONEXPORT.debug(str)};"
        ) 
      .arg(-100)
      .arg(100)
      .arg(QValidator::State::Intermediate)
      .arg(QValidator::State::Acceptable)
      .arg(QValidator::State::Invalid));

  jsengin.globalObject().setProperty("GLOBALEXPORT", jsobject);

  jsobject = jsengin.evaluate(script);

  if(jsobject.isError())
  {
    qDebug() 
      << "Uncaught exception at line"
      << jsobject.property("lineNumber").toInt()
      << ":" << jsobject.toString();
  }

  QJSValue jsvalidate = jsengin.globalObject().property("validate");
  /* QJSValue jsvalidate; */
  /* LCQJsValidator jsvalidator(jsvalidate); */
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


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
#include "jsclass.h"

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

  QJSEngine jsengin;
  QJSValue jsappinterface = jsengin.newQObject(appinterface);
  jsengin.globalObject().setProperty("Application", jsappinterface);

  QJSValue jsobject = jsengin.evaluate(script);

  if(jsobject.isError())
  {
    qDebug() 
      << "Uncaught exception at line"
      << jsobject.property("lineNumber").toInt()
      << ":" << jsobject.toString();
  }

  QJSValue jsvalidate = jsengin.globalObject().property("validate");

  QObject::connect(line_edit1, &QLineEdit::textChanged, 
      [label1, &jsvalidate](const QString& _text)
      {
        label1->setText(_text);
        jsvalidate.call(QJSValueList() << _text);
      });

  layout->addWidget(label1);
  layout->addWidget(line_edit1);
  widget->setLayout(layout);

  widget->show();
  return app.exec();
}

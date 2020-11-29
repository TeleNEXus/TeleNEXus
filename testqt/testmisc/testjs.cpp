
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
  QLabel* label2 = new QLabel("Label 2");
  /* QByteArray* array1 = new QByteArray(4,'a'); */


  QString fileName = "../script1.js";
  QFile scriptFile(fileName);
  QString script;
  if (!scriptFile.open(QIODevice::ReadOnly)) qDebug() << "Catn't open file";
  else
  {
    QTextStream stream(&scriptFile);
    script = stream.readAll();
    scriptFile.close();
  }
  LJsClass *jsinterface = new LJsClass;

  QJSEngine jsengin;
  QJSValue jslabel1= jsengin.newQObject(label1);
  QJSValue jslabel2= jsengin.newQObject(label2);
  QJSValue jsclass= jsengin.newQObject(jsinterface);

  jsengin.globalObject().setProperty("jslabel1", jslabel1);
  jsengin.globalObject().setProperty("jslabel2", jslabel2);
  jsengin.globalObject().setProperty("jsint", jsclass);

  /* jsengin.evaluate( */
  /*     "jslabel1.setText('jslabel 1++');jslabel2.setText('jslabel 2++');" */
  /*     ); */
  /* jsengin.evaluate( */
  /*     "jslabel1.setText('jslabel 1++');" */
  /*     "jslabel2.setText('jslabel 2++++++');" */
  /*     ); */

  jsengin.evaluate(script);

  layout->addWidget(label1);
  layout->addWidget(label2);
  widget->setLayout(layout);

  widget->show();
  return app.exec();
}

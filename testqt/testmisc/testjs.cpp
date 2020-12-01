
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
  QPushButton* button_start = new QPushButton("Start");
  QPushButton* button_stop = new QPushButton("Stop");
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
  jsengin.globalObject().setProperty("appifc", jsclass);

  /* jsengin.evaluate( */
  /*     "jslabel1.setText('jslabel 1++');jslabel2.setText('jslabel 2++');" */
  /*     ); */
  /* jsengin.evaluate( */
  /*     "jslabel1.setText('jslabel 1++');" */
  /*     "jslabel2.setText('jslabel 2++++++');" */
  /*     ); */

  /* jsengin.evaluate(script); */
  /* script = "var i=0; str = new String; str = i; appifc.print('script print ' + str);"; */
  script = "var i=0; str = new String; while(i < 100000){str = i; i++; appifc.print('script print ' + str);}";
  /* jsengin.evaluate("appifc.print('script print');"); */
  jsengin.evaluate(script);

  QThread* th1 = new QThread;
  QTimer* timer = new QTimer;
  timer->moveToThread(th1);
  int timer_tick_counter = 0;
  QObject::connect(th1, &QThread::started, 
      [&]
      {
      timer->start(500);
      });
  QObject::connect(th1, &QThread::finished, 
      [&]
      {
      timer->stop();
      });
  QObject::connect(timer, &QTimer::timeout,
      [&]
      {
      qDebug() << "Timer tick " << timer_tick_counter;
      timer_tick_counter++;
      /* jsengin.evaluate("appifc.print('script print');"); */
      jsengin.evaluate(script);
      });

  QObject::connect(button_start, &QPushButton::pressed, 
      [&]
      {
      label1->setText("Push Button Start");
      th1->start();
      });
  QObject::connect(button_stop, &QPushButton::pressed, 
      [&]
      {
      label1->setText("Push Button Stop");
      th1->quit();
      });

  layout->addWidget(label1);
  layout->addWidget(label2);
  layout->addWidget(button_start);
  layout->addWidget(button_stop);
  widget->setLayout(layout);

  widget->show();
  return app.exec();
}

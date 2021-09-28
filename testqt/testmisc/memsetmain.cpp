#include <QCoreApplication>
#include <QDebug>
#include "lcmemoryset.h"
int main(int argc, char** argv)
{
  Q_UNUSED(argc);
  Q_UNUSED(argv);
  /* QCoreApplication app(argc, argv); */
  /* return app.exec(); */


  qDebug() << "===========================================";
  qDebug() << "Test Memory set";
  qDebug() << "===========================================";
  LCMemorySet memoryset(6);

  return 0;
}

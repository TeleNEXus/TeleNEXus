#ifndef LCQJSAPPLICATIONINTERFACE_H_
#define LCQJSAPPLICATIONINTERFACE_H_

#include <QObject>

class LCQJSApplicationInterface : public QObject
{
  Q_OBJECT
private:

public:
  LCQJSApplicationInterface();
  ~LCQJSApplicationInterface();

public slots:
  void printapp(const QString& _str);
};

#endif


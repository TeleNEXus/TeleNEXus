#ifndef LCQJSAPPLICATIONINTERFACE_H_
#define LCQJSAPPLICATIONINTERFACE_H_

#include <QObject>
class LIApplication;

class LCQJSApplicationInterface : public QObject
{
  Q_OBJECT
private:

  const LIApplication& edApplication;

  LCQJSApplicationInterface(const LIApplication& _app);
  LCQJSApplicationInterface() = delete;
  /* LCQJSApplicationInterface(const LCQJSApplicationInterface&) = delete; */
  /* LCQJSApplicationInterface& operator=(const LCQJSApplicationInterface&) = delete; */
public:
  /* static LCQJSApplicationInterface& instance(); */
public slots:
  void print(const QString& _str);
};

#endif


#ifndef LCQJSAPPLICATIONINTERFACE_H_
#define LCQJSAPPLICATIONINTERFACE_H_

#include <QObject>
class LIApplication;

class LCQJSAppInterface : public QObject
{
  Q_OBJECT
private:
    const LIApplication& edApplication;
public:
  LCQJSAppInterface();
  ~LCQJSAppInterface();

public slots:
  void print(QString _str);
};

#endif


#ifndef LCQJSAPPSERVICE_H_
#define LCQJSAPPSERVICE_H_

#include "lqextendevent.h"

#include <QObject>
#include <QSharedPointer>
#include <QEvent>
#include <QMutex>
#include <QWaitCondition>

class QThread;
class LIApplication;

class LCQJSAppService final 
{
private:
  QThread* mpThread;
  LCQJSAppService();
  LCQJSAppService(const LCQJSAppService&) = delete;
  LCQJSAppService& operator=(const LCQJSAppService&) = delete;
public:

  ~LCQJSAppService();
  static QSharedPointer<LCQJSAppService> getService();

  QString readSourceData(QString _dataId);
  QString getProjectPath();

};
#endif

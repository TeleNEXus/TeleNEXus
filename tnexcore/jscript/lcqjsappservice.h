#ifndef LCQJSAPPSERVICE_H_
#define LCQJSAPPSERVICE_H_

#include <QObject>
#include <QSharedPointer>

class LCQJSAppService : private QObject
{
  Q_OBJECT
private:
  QWeakPointer<LCQJSAppService> mwpService;
  LCQJSAppService(QObject* _parent);
  LCQJSAppService(const LCQJSAppService&) = delete;
  LCQJSAppService& operator=(const LCQJSAppService&) = delete;
public:
  static QSharedPointer<LCQJSAppService> getService();
};
#endif

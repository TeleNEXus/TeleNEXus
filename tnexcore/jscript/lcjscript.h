#ifndef LCJSCRIPT_H_
#define LCJSCRIPT_H_

#include <QString>

class LCJScript
{
private:
  void* mpData;
public:
  explicit LCJScript(const QString& _script = QString(), int _interval = -1);
  ~LCJScript();
  void start();
  void stop();
};

#endif


#ifndef LCJSCRIPT_H_
#define LCJSCRIPT_H_

#include "LIJScript.h"
#include <QString>

class LCJScript : public LIJScript
{
private:
  void* mpData;
public:
  explicit LCJScript(const QString& _script = QString(), int _interval = -1);
  ~LCJScript();
  virtual void start() const override;
  virtual void stop() const override;
};

#endif


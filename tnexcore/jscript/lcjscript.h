#ifndef LCJSCRIPT_H_
#define LCJSCRIPT_H_

#include "LIJScript.h"
#include <QString>

class LCJScript : public LIJScript
{
private:
  void* mpData;
public:
  explicit LCJScript(const QString& _script, int _interval = -1);
  virtual ~LCJScript();
  virtual void start() override;
  virtual void stop() override;
};

#endif


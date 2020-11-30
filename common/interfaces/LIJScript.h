#ifndef LIJSCRIPT_H_
#define LIJSCRIPT_H_

class LIJScript
{
public:
  LIJScript(){}
  virtual ~LIJScript(){}
  virtual void start() = 0;
  virtual void stop() = 0;
};

#endif


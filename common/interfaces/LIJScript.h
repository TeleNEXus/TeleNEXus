#ifndef LIJSCRIPT_H_
#define LIJSCRIPT_H_

class LIJScript
{
public:
  LIJScript(){}
  virtual void start() const = 0;
  virtual void stop() const = 0;
};

#endif


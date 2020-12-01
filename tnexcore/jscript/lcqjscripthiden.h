#ifndef LCQJSCRIPTHIDEN_H_
#define LCQJSCRIPTHIDEN_H_

#include <QObject>
#include "lqextendevent.h"

class LCQJScriptHiden : public QObject
{
  Q_OBJECT
private:

    //--------------------------------------------------------------------------CEventBase
    class CEventBase : public QEvent
    {
        __LQ_EXTENDED_QEVENT_DECLARATION
    public:
        CEventBase();
        virtual void handle(LCQJScriptHiden* _sender) = 0;
    };

    //--------------------------------------------------------------------------CEventStart
    class CEventStart: public CEventBase
    {
    private:
      int mInterval;
    public:
        CEventStart() = delete;
        CEventStart(int _interval);
        virtual void handle(LCQJScriptHiden* _sender) override;
    };

    //--------------------------------------------------------------------------CEventStop
    class CEventStop: public CEventBase
    {
    public:
        CEventStop();
        virtual void handle(LCQJScriptHiden* _sender) override;
    };

    void* mpData;

public:
    static int mObjectCounter;
  explicit LCQJScriptHiden(const QString& _script, QObject* _parent = nullptr);
  virtual ~LCQJScriptHiden();
  virtual void customEvent(QEvent* _event) override;
  void start(int interval);
  void stop();
};
#endif

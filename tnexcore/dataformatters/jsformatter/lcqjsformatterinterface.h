#ifndef LCQJSFORMATTERINTERFACE_H_
#define LCQJSFORMATTERINTERFACE_H_

#include <QObject>
#include <QJSEngine>

class LCQJSFormatterInterface final: public QObject
{
  Q_OBJECT;

  LCQJSFormatterInterface();
  LCQJSFormatterInterface(const LCQJSFormatterInterface&) = delete;
  LCQJSFormatterInterface& operator=(const LCQJSFormatterInterface&) = delete;
public:
  ~LCQJSFormatterInterface();

  LCQJSFormatterInterface& getInstance();
  static void setProperty(const QString& _propName, QJSEngine& _jsengine);

public slots:
  void debugOut(const QString&);

};
#endif  //LCQJSFORMATTERINTERFACE_H_

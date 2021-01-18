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
  static QSharedPointer<LCQJSFormatterInterface> create();

public slots:
  void debugOut(const QString&);
  void exportModule(const QString& _moduleFileName);

};
#endif  //LCQJSFORMATTERINTERFACE_H_

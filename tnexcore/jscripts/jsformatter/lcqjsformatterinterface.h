#ifndef LCQJSFORMATTERINTERFACE_H_
#define LCQJSFORMATTERINTERFACE_H_

#include <QObject>
/* #include <QJSEngine> */
class QJSEngine;
class LCQJSFormatterInterface final: public QObject
{
  Q_OBJECT;

private:
  QJSEngine* mpEngine;
  LCQJSFormatterInterface(QJSEngine* _engine);
  LCQJSFormatterInterface() = delete;
  LCQJSFormatterInterface(const LCQJSFormatterInterface&) = delete;
  LCQJSFormatterInterface& operator=(const LCQJSFormatterInterface&) = delete;

public:
  ~LCQJSFormatterInterface();
  static QSharedPointer<LCQJSFormatterInterface> 
    create(QJSEngine* _engine);

public slots:
  void message(const QString&);
  void warning(const QString&);
  void error(const QString&);
void importModule(
    const QString& _fileName,
    const QString& _propertyName = QString());


};
#endif  //LCQJSFORMATTERINTERFACE_H_

#ifndef LCQJSAPPLICATIONINTERFACE_H_
#define LCQJSAPPLICATIONINTERFACE_H_

#include <QObject>
#include <QVariantList>

class LIApplication;

class LCQJSAppInterface : public QObject
{
  Q_OBJECT
private:
   void* mpData; 
public:
  LCQJSAppInterface();
  ~LCQJSAppInterface();

public slots:

  void debug(const QString& _str);

  QString getProjectPath();

  QVariantList readData(const QString& _sourceId, const QString& _dataId);

  int writeData(
      const QString& _sourceId, 
      const QString& _dataId, 
      const QVariantList& _data);

  QVariantList testGetByteArray();
};

#endif


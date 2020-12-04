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
  void print(QString _str);
  QString getProjectPath();
  QVariantList readSourceData(const QString& _sourceId, const QString& _dataId);
  QVariantList testGetByteArray();
};

#endif


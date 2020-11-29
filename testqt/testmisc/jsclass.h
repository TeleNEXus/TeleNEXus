#ifndef LSCLASS_H_
#define LSCLASS_H_
#include <QObject>
#include <QVariant>
#include <QVector>
#include <QVariantList>
class LJsClass : public QObject
{
  Q_OBJECT
private:
    char* mCh;
public:
    explicit LJsClass(QObject* _parent = nullptr);
    virtual ~LJsClass();

public slots:
  QString getJsClassName();
  QString getFullFileName(const QString& _file);
  QVariantList getArray();
  QVariantList getArray(const QVariantList _list);
    
};
#endif


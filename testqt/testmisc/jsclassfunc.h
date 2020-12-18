#ifndef LSCLASS_H_
#define LSCLASS_H_
#include <QObject>
#include <QVariant>
#include <QVector>
#include <QVariantList>
class LJsClass : public QObject
{
  Q_OBJECT
public:
    explicit LJsClass(QObject* _parent = nullptr);
    virtual ~LJsClass();

public slots:
  void debug(const QString&);
    
};
#endif


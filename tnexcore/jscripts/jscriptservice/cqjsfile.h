#ifndef CQJSFILE_H_
#define CQJSFILE_H_

#include <QJSEngine>

class CQJSFileBase
{
public:
  CQJSFileBase();

public slots:
  bool open(const QString& _fileName, const QString& _openMode);
  void close();
  bool exists();
  bool remove();
  bool rename(const QString& _newName);
};

/* class CQJSFileBase */
/* { */
/* protected: */
/*   QJSEngine* mpOwnerEngine = nullptr; */

/* public: */
/*   /1* Q_INVOKABLE explicit CQJSFile(QObject* _parent = 0); *1/ */
/*   Q_INVOKABLE explicit CQJSFileBase(QJSEngine* _owner); */

/* public slots: */
/*   bool open(const QString& _fileName, const QString& _openMode); */
/*   void close(); */

/*   int write(const QJSValue& _data); */ 
/*   QJSValue read(int _length = 0); */

/*   bool exists(); */
/*   bool remove(); */
/*   bool rename(const QString& _newName); */

/* }; */

#endif //CQJSFILE_H_

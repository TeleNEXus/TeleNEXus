#ifndef LSCLASS_H_
#define LSCLASS_H_
#include <QObject>
#include <QVariant>
#include <QVector>
#include <QVariantList>

enum class EValidateState
{
  intermediate,
  acceptable,
  invalid
};

/* class LQValidateStatus : public QObject */
/* { */
/*   Q_OBJECT; */
/* public: */
/*   Q_PROPERTY(EValidateState intermediate READ getIntermediate SCRIPTABLE true); */ 
/*   Q_PROPERTY(EValidateState acceptable READ getAcceptable SCRIPTABLE true); */ 
/*   Q_PROPERTY(EValidateState invalid READ getInvalid SCRIPTABLE true); */ 

/* public: */
/*   explicit LQValidateStatus(): QObject(nullptr) {} */
/*   EValidateState getIntermediate() { return (EValidateState::intermediate);} */
/*   EValidateState getAcceptable() { return   (EValidateState::acceptable);} */
/*   EValidateState getInvalid() { return      (EValidateState::invalid);} */
/* }; */


class LJsClass : public QObject
{
  Q_OBJECT;
  Q_PROPERTY(EValidateState Intermediate  READ getIntermediate SCRIPTABLE true); 
  Q_PROPERTY(EValidateState Acceptable    READ getAcceptable SCRIPTABLE true); 
  Q_PROPERTY(EValidateState Invalid       READ getInvalid SCRIPTABLE true); 
public:
  explicit LJsClass(QObject* _parent = nullptr);
  virtual ~LJsClass();

  EValidateState getIntermediate()  { return EValidateState::intermediate;}
  EValidateState getAcceptable()    { return EValidateState::acceptable;}
  EValidateState getInvalid()       { return EValidateState::invalid;}

  public slots:
  void debug(const QString&);
};

Q_DECLARE_METATYPE(EValidateState);

#endif


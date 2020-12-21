#ifndef LSCLASS_H_
#define LSCLASS_H_
#include <QObject>
#include <QVariant>
#include <QVector>
#include <QVariantList>

enum class EJSValidateState
{
  intermediate,
  acceptable,
  invalid
};

/* class LQValidateStatus : public QObject */
/* { */
/*   Q_OBJECT; */
/* public: */
/*   Q_PROPERTY(EJSValidateState intermediate READ getIntermediate SCRIPTABLE true); */ 
/*   Q_PROPERTY(EJSValidateState acceptable READ getAcceptable SCRIPTABLE true); */ 
/*   Q_PROPERTY(EJSValidateState invalid READ getInvalid SCRIPTABLE true); */ 

/* public: */
/*   explicit LQValidateStatus(): QObject(nullptr) {} */
/*   EJSValidateState getIntermediate() { return (EJSValidateState::intermediate);} */
/*   EJSValidateState getAcceptable() { return   (EJSValidateState::acceptable);} */
/*   EJSValidateState getInvalid() { return      (EJSValidateState::invalid);} */
/* }; */


class LJsClass : public QObject
{
  Q_OBJECT;
  Q_PROPERTY(EJSValidateState Intermediate  READ getIntermediate SCRIPTABLE true); 
  Q_PROPERTY(EJSValidateState Acceptable    READ getAcceptable SCRIPTABLE true); 
  Q_PROPERTY(EJSValidateState Invalid       READ getInvalid SCRIPTABLE true); 
public:
  explicit LJsClass(QObject* _parent = nullptr);
  virtual ~LJsClass();

  EJSValidateState getIntermediate()  { return EJSValidateState::intermediate;}
  EJSValidateState getAcceptable()    { return EJSValidateState::acceptable;}
  EJSValidateState getInvalid()       { return EJSValidateState::invalid;}

  public slots:
  void debug(const QString&);
};

Q_DECLARE_METATYPE(EJSValidateState);

#endif


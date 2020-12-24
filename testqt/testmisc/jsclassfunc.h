#ifndef LSCLASS_H_
#define LSCLASS_H_
#include <QObject>
#include <QVariant>
#include <QVector>
#include <QVariantList>

/* enum class EJSValidateState */
/* { */
/*   intermediate, */
/*   acceptable, */
/*   invalid */
/* }; */

/* /1* class LQValidateStatus : public QObject *1/ */
/* /1* { *1/ */
/* /1*   Q_OBJECT; *1/ */
/* /1* public: *1/ */
/* /1*   Q_PROPERTY(EJSValidateState intermediate READ getIntermediate SCRIPTABLE true); *1/ */ 
/* /1*   Q_PROPERTY(EJSValidateState acceptable READ getAcceptable SCRIPTABLE true); *1/ */ 
/* /1*   Q_PROPERTY(EJSValidateState invalid READ getInvalid SCRIPTABLE true); *1/ */ 

/* /1* public: *1/ */
/* /1*   explicit LQValidateStatus(): QObject(nullptr) {} *1/ */
/* /1*   EJSValidateState getIntermediate() { return (EJSValidateState::intermediate);} *1/ */
/* /1*   EJSValidateState getAcceptable() { return   (EJSValidateState::acceptable);} *1/ */
/* /1*   EJSValidateState getInvalid() { return      (EJSValidateState::invalid);} *1/ */
/* /1* }; *1/ */


/* class LJsClass : public QObject */
/* { */
/*   Q_OBJECT; */
/*   Q_PROPERTY(EJSValidateState Intermediate  READ getIntermediate SCRIPTABLE true); */ 
/*   Q_PROPERTY(EJSValidateState Acceptable    READ getAcceptable SCRIPTABLE true); */ 
/*   Q_PROPERTY(EJSValidateState Invalid       READ getInvalid SCRIPTABLE true); */ 
/* public: */
/*   explicit LJsClass(QObject* _parent = nullptr); */
/*   virtual ~LJsClass(); */

/*   EJSValidateState getIntermediate()  { return EJSValidateState::intermediate;} */
/*   EJSValidateState getAcceptable()    { return EJSValidateState::acceptable;} */
/*   EJSValidateState getInvalid()       { return EJSValidateState::invalid;} */

/*   public slots: */
/*   void debug(const QString&); */
/* }; */

/* Q_DECLARE_METATYPE(EJSValidateState); */


class LJsClass : public QObject
{
  Q_OBJECT;
public:
  explicit LJsClass(QObject* _parent = nullptr);
  virtual ~LJsClass();

  public slots:
  void debug(const QString&);
};

#endif


#ifndef JSVALIDATOR_H_
#define JSVALIDATOR_H_

#include <QValidator>
#include <QJSValue>

class LCQJsValidator : public QValidator
{
private:
  QJSValue mCallObject;
public:
  explicit LCQJsValidator(const QJSValue& _callObject, QObject* _parent = nullptr);
  virtual State validate(QString& _input, int& _pos) const;
};

#endif //JSVALIDATOR_H_


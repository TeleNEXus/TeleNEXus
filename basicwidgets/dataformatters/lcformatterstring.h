#ifndef LCFORMATTERSTRING_H
#define LCFORMATTERSTRING_H

#include "lcformatterbase.h"
#include <QValidator>

class LCFormatterString : public LCFormatterBase
{
public:
  explicit LCFormatterString();

  virtual QString     toString(const QByteArray& _data) override;
  virtual QString     normalize(const QString& _str) override;
  virtual QByteArray  toBytes(const QString& _str) override;
  virtual QValidator* validator() override;
};

#endif // LCFORMATTERSTRING_H

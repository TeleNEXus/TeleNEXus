#ifndef LCSTRINGDATAFORMATTERUINT8_H
#define LCSTRINGDATAFORMATTERUINT8_H

#include "lcformatterintbase.h"
#include <QValidator>

class LCFormatterU8 : public LCFormatterIntBase
{
private:

  QIntValidator mValidator;   //Контроллер диапазона ввода.

public:
  explicit LCFormatterU8(    
      int     _fieldWidth     = 0,
      QChar   _fillChar       = QChar(' '),
      int     _base           = 10);

  virtual QString     toString(const QByteArray& _data) override;
  virtual QString     normalize(const QString& _str) override;
  virtual QByteArray  toBytes(const QString& _str) override;
  virtual QValidator* validator() override;
};

#endif // LCSTRINGDATAFORMATTERUINT8_H

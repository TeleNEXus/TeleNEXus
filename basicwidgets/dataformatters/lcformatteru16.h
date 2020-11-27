#ifndef LCSTRINGDATAFORMATTERUINT16_H
#define LCSTRINGDATAFORMATTERUINT16_H

#include "lcformatterintbase.h"
#include <QIntValidator>

class LCFormatterU16 : public LCFormatterIntBase
{
private:
  QIntValidator mValidator;   //Контроллер диапазона ввода.

public:
  explicit LCFormatterU16(  
      int     _fieldWidth     = 0,
      QChar   _fillChar       = QChar(' '),
      int     _base           = 10);

  virtual QString     toString(const QByteArray& _data) override;
  virtual QString     normalize(const QString& _str) override;
  virtual QByteArray  toBytes(const QString& _str) override;
  virtual QValidator* validator() override;
};

#endif // LCSTRINGDATAFORMATTERUINT16_H

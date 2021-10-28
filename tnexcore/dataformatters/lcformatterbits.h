/* 
 * TeleNEXus is a simple SCADA programm
 *
 * Copyright (C) 2020 Sergey S. Kuzmenko
 *
 * This file is part of TeleNEXus.
 *
 * TeleNEXus is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * TeleNEXus is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with TeleNEXus.  If not, see <https://www.gnu.org/licenses/>.
 */
#ifndef LCFORMATTERBITS_H
#define LCFORMATTERBITS_H

#include "lcformatterbase.h"
#include <QValidator>

class LCFormatterBits : public LCFormatterBase
{
private:

  class CValidator : public QValidator
  {
  public:
    int      mSize;
    QChar    mSeparator;

    explicit CValidator(
        int _size = 0, 
        QChar _separator = QChar(), 
        QObject *_parent = nullptr);
    virtual ~CValidator(){}

    virtual QValidator::State validate(
        QString &_input, 
        int& _pos) const override;
  };

private:
  CValidator  mValidator;
public:

  explicit LCFormatterBits(    
      int _size = 0, 
      QChar _separator = QChar());

  explicit LCFormatterBits( 
      const LCFormatterBits& _formatter);
  virtual ~LCFormatterBits();

  LCFormatterBits& operator=(
      const LCFormatterBits& _formatter);

  virtual QString     toString(const QByteArray& _data) override;
  virtual QByteArray  toBytes(const QString& _str) override;
  virtual QValidator* validator() override {return &mValidator;} 
  void setSize(int _size);
  void setSeparator(QChar _separator);
};

#endif // LCFORMATTERBITS_H



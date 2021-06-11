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
#ifndef LCFORMATTERFLOAT32_H
#define LCFORMATTERFLOAT32_H

#include "lcformatterbase.h"
#include <QDoubleValidator>


class LCFormatterF32 : public LCFormatterBase
{
private:
    int     mFieldWidth;        //Ширина поля.
    char    mFormat;
    int     mPrecision;
    QChar   mFillChar;          //Символ заполнения выравнивания.

    QDoubleValidator mValidator;   //Контроллер диапазона ввода.

public:
    explicit LCFormatterF32(    int     _fieldWidth     = 0,
                                          char    _format         = 'g',
                                          int     _precision      = -1,
                                          int     _decimals       = 1000,
                                          QChar   _fillChar       = QChar(' '));
    virtual ~LCFormatterF32();
    virtual QString     toString(const QByteArray& _data) override;
    virtual QByteArray  toBytes(const QString& _str) override;
    virtual QValidator* validator() override;

    void setFieldWidth(int _fieldWidth){mFieldWidth = _fieldWidth;}
    void setFillChar(QChar _c){mFillChar = _c;}
    void setFormat(char _format){mFormat = _format;}
    void setPrecision(int _precision){mPrecision = _precision;}
};


#endif // LCFORMATTERFLOAT32_H

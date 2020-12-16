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
#ifndef LCSTRINGDATAFORMATTERBITFIELD_H 
#define LCSTRINGDATAFORMATTERBITFIELD_H 

#include "lcformatterbase.h"
#include <QValidator>

class LCFormatterBitfield : public LCFormatterBase
{
private:
    //Валидатор вводимых данных.
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
    CValidator  mValidator;       //Валидатор.
public:

    explicit LCFormatterBitfield(
        int _size = 0, 
        QChar _separator = QChar());

    explicit LCFormatterBitfield( 
            const LCFormatterBitfield& _formatter);

    virtual ~LCFormatterBitfield();

    LCFormatterBitfield& operator=(
            const LCFormatterBitfield& _formatter);

    virtual QString     toString(const QByteArray& _data) override;
    virtual QString     fitting(const QString& _str) override;
    virtual QByteArray  toBytes(const QString& _str) override;
    virtual QValidator* validator() override {return &mValidator;}

    void setSize(int _size);
    void setSeparator(QChar _separator);
};


#endif // LCSTRINGDATAFORMATTERBITFIELD_H

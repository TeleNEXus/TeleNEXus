#ifndef LCSTRINGDATASTRFORMATTERBASEINT_H
#define LCSTRINGDATASTRFORMATTERBASEINT_H

#include "lcformatterbase.h"

class LCFormatterIntBase : public LCFormatterBase
{
protected:

    int     mFieldWidth;        //Ширина поля.
    QChar   mFillChar;          //Символ заполнения выравнивания.
    int     mBase;              //Основание системы счисления.

public:
    LCFormatterIntBase() = delete;
    explicit LCFormatterIntBase( 
        int     _fieldWidth,
        QChar   _fillChar,
        int     _base);

    virtual ~LCFormatterIntBase();
    void setFieldWidth(int _fieldWidth){mFieldWidth = _fieldWidth;}
    void setFillChar(QChar _c){mFillChar = _c;}
    void setBase(int _base){mBase = _base;}
};


#endif // LCSTRINGDATASTRFORMATTERBASEINT_H

#ifndef LCSTRINGDATASTRFORMATTERBASEINT_H
#define LCSTRINGDATASTRFORMATTERBASEINT_H

#include "lcstringdataformatterbase.h"

class LCStringDataFormatterIntBase : public LCStringDataFormatterBase
{
protected:

    int     mFieldWidth;        //Ширина поля.
    QChar   mFillChar;          //Символ заполнения выравнивания.
    int     mBase;              //Основание системы счисления.

public:
    LCStringDataFormatterIntBase() = delete;
    explicit LCStringDataFormatterIntBase( 
        int     _fieldWidth,
        QChar   _fillChar,
        int     _base);

    ~LCStringDataFormatterIntBase();
    void setFieldWidth(int _fieldWidth){mFieldWidth = _fieldWidth;}
    void setFillChar(QChar _c){mFillChar = _c;}
    void setBase(int _base){mBase = _base;}
};


#endif // LCSTRINGDATASTRFORMATTERBASEINT_H

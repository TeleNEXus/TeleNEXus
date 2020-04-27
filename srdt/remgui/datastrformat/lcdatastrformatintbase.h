#ifndef LCDATASTRFORMATINTBASE_H
#define LCDATASTRFORMATINTBASE_H

#include "lcdatastrformatbase.h"

namespace remgui
{

class LCDataStrFormatIntBase : public LCDataStrFormatBase
{
protected:

    int     mFieldWidth;        //Ширина поля.
    QChar   mFillChar;          //Символ заполнения выравнивания.
    int     mBase;              //Основание системы счисления.
    QChar   mFillCharUndef;     //Заполнение при неопределеном значении.
    QChar   mFillCharWrong;     //Заполнение при ошибочном значении.

public:
    LCDataStrFormatIntBase() = delete;
    explicit LCDataStrFormatIntBase( int     _fieldWidth,
                                     QChar   _fillChar,
                                     int     _base,
                                     QChar   _fillCharUndef,
                                     QChar   _fillCharWrong);

    void setFieldWidth(int _fieldWidth){mFieldWidth = _fieldWidth;}
    void setFillChar(QChar _c){mFillChar = _c;}
    void setBase(int _base){mBase = _base;}
    void setFillCharUndef(QChar _c){mFillCharUndef = _c;}
    void setFillCharWrong(QChar _c){mFillCharWrong = _c;}
};

} //namespace

#endif // LCDATASTRFORMATINTBASE_H

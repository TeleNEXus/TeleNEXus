#ifndef LCDATASTRINGFORMATTERUINT32_H
#define LCDATASTRINGFORMATTERUINT32_H

#include "lcdatastrformatintbase.h"
#include "lcqu32validator.h"

namespace remgui
{

class LCDataStrFormatU32 : public LCDataStrFormatIntBase
{
private:

    LCQValidatorU32 mValidator;   //Контроллер диапазона ввода.

public:
    explicit LCDataStrFormatU32( int     _fieldWidth     = 0,
                                          QChar   _fillChar       = QChar(' '),
                                          int     _base           = 10,
                                          QChar   _fillCharUndef  = QChar(),
                                          QChar   _fillCharWrong  = QChar());

    virtual QString     toString(const QByteArray& _data) override;
    virtual QByteArray  toBytes(const QString& _str) override;
    virtual QString     undefStateString() override;
    virtual QValidator* validator() override;
};

}

#endif // LCDATASTRINGFORMATTERUINT32_H

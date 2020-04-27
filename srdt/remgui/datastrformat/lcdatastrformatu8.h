#ifndef LCDATASTRINGFORMATTERUINT8_H
#define LCDATASTRINGFORMATTERUINT8_H

#include "lcdatastrformatintbase.h"

namespace remgui
{

class LCDataStrFormatU8 : public LCDataStrFormatIntBase
{
private:

    QIntValidator mValidator;   //Контроллер диапазона ввода.

public:
    explicit LCDataStrFormatU8(    int     _fieldWidth     = 0,
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

#endif // LCDATASTRINGFORMATTERUINT8_H

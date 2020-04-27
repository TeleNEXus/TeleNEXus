#ifndef LCDATASTRINGFORMATTERINT8_H
#define LCDATASTRINGFORMATTERINT8_H

#include "lcdatastrformatintbase.h"

namespace remgui
{

class LCDataStrFormatS8 : public LCDataStrFormatIntBase
{
private:

    QIntValidator mValidator;   //Контроллер диапазона ввода.

public:
    explicit LCDataStrFormatS8(    int     _fieldWidth     = 0,
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

#endif // LCDATASTRINGFORMATTERINT8_H

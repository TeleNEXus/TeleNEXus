#ifndef LCDATASTRINGFORMATTERINT16_H
#define LCDATASTRINGFORMATTERINT16_H


#include "lcdatastrformatintbase.h"
#include <QIntValidator>

namespace remgui
{

class LCDataStrFormatS16 : public LCDataStrFormatIntBase
{
private:

    QIntValidator mValidator;   //Контроллер диапазона ввода.

public:
    explicit LCDataStrFormatS16(    int     _fieldWidth     = 0,
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

#endif // LCDATASTRINGFORMATTERINT16_H

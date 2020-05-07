#ifndef LCSTRINGDATAFORMATTERBASE_H
#define LCSTRINGDATAFORMATTERBASE_H

#include <QString>
#include <QByteArray>
#include <QValidator>

#include "LCStringDataFormatterInterface.h"


class LCStringDataFormatterBase : public LCStringDataFormatterInterface
{
protected:
    static QChar  msFillCharUndefDef;         //Символы заполнения при неопределенных данных.
    static quint8 msFillCharUndefDefLength;   //Длина заполнения при неопределенных данных.
    static QChar  msFillCharWrongDef;         //Символы заполнения при ошибочных данных.
    static quint8 msFillCharWrongDefLength;   //Длина заполнения при ошибочных данных.
public:
    LCStringDataFormatterBase();
    virtual ~LCStringDataFormatterBase();

    virtual QString getUndefStateString(int _fieldWidth = msFillCharUndefDefLength,
                                        QChar _charUndef = msFillCharUndefDef) override;
    virtual QString getWrongStateString(int _fieldWidth = msFillCharWrongDefLength,
                                        QChar _charWrong = msFillCharWrongDef) override;
    virtual void setFillCharUndefDef(QChar _c, quint8 _length) override;
    virtual void setFillCharWrongDef(QChar _c, quint8 _length) override;
};


#endif //LCSTRINGDATAFORMATTERBASE_H

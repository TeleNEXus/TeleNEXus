#ifndef LCQDATASTRINGFORMATTERBASE_H
#define LCQDATASTRINGFORMATTERBASE_H

#include <QString>
#include <QByteArray>
#include <QValidator>
#include "lremgui.h"

namespace remgui
{

class LCDataStrFormatBase
{
protected:
    static QChar  msFillCharUndefDef;         //Символы заполнения при неопределенных данных.
    static quint8 msFillCharUndefDefLength;   //Длина заполнения при неопределенных данных.
    static QChar  msFillCharWrongDef;         //Символы заполнения при ошибочных данных.
    static quint8 msFillCharWrongDefLength;   //Длина заполнения при ошибочных данных.
public:
    LCDataStrFormatBase();
    virtual QString toString(const QByteArray& _data) = 0;
    virtual QByteArray toBytes(const QString& _str) = 0;
    virtual QString undefStateString()=0;
    virtual QValidator* validator() = 0;

public:

    static QString getUndefStateString(int _fieldWidth = msFillCharUndefDefLength,
                                        QChar _charUndef = msFillCharUndefDef);

    static QString getWrongStateString(int _fieldWidth = msFillCharWrongDefLength,
                                        QChar _charWrong = msFillCharWrongDef);
public:
    static void setFillCharUndefDef(QChar _c, quint8 _length);
    static void setFillCharUndefDef(QChar _c);
    static void setFillCharWrongDef(QChar _c, quint8 _length);
    static void setFillCharWrongDef(QChar _c);
};

}

#endif // LCQDATASTRINGFORMATTERBASE_H

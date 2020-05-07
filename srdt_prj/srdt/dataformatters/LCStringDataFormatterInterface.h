#ifndef LCSTRINGDATAFORMATTERINTERFACE_H
#define LCSTRINGDATAFORMATTERINTERFACE_H

#include <qglobal.h>

class QByteArray;
class QString;
class QValidator;
class QChar;



class LCStringDataFormatterInterface
{

public:

    LCStringDataFormatterInterface(){}
    ~LCStringDataFormatterInterface(){}

    virtual QString toString(const QByteArray& _data) = 0;
    virtual QByteArray toBytes(const QString& _str) = 0;
    virtual QString undefStateString()=0;
    virtual QValidator* validator() = 0;
    virtual QString getUndefStateString(int _fieldWidth, QChar _charUndef) = 0;
    virtual QString getWrongStateString(int _fieldWidth, QChar _charWrong) = 0;
    virtual void setFillCharUndefDef(QChar _c, quint8 _length) = 0;
    virtual void setFillCharWrongDef(QChar _c, quint8 _length) = 0;
};

#endif // LCSTRINGDATAFORMATTERINTERFACE_H

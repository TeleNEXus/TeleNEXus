#ifndef LCSTRINGDATAFORMATTERINTERFACE_H
#define LCSTRINGDATAFORMATTERINTERFACE_H

#include <qglobal.h>

class QByteArray;
class QString;
class QValidator;
class QChar;

class LIDataFormatter
{

public:

    LIDataFormatter(){}
    virtual ~LIDataFormatter(){}

    virtual QString toString(const QByteArray& _data) = 0;
    virtual QByteArray toBytes(const QString& _str) = 0;
    virtual QString normalize(const QString& _str) = 0;
    virtual bool undefState(QString& _string) =0;
    virtual bool wrongState(QString& _string) = 0;
    virtual QValidator* validator() = 0;
};

#endif // LCSTRINGDATAFORMATTERINTERFACE_H
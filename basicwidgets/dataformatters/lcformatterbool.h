#ifndef LCDATAFORMATBOOL_H
#define LCDATAFORMATBOOL_H

#include "lcformatterbase.h"
#include <QValidator>

class LCFormatterBool : public LCFormatterBase
{
private:
    QString mStrFalse;
    QString mStrTrue;
public:
    explicit LCFormatterBool(   const QString& _strFalse = "false",
                                    const QString& _strTrue = "true");
    virtual ~LCFormatterBool(){}
    virtual QString     toString(const QByteArray& _data) override;
    virtual QString     normalize(const QString& _str) override;
    virtual QByteArray  toBytes(const QString& _str) override;
    virtual QValidator* validator() override {return nullptr;}

    void setFalseString(const QString _str){mStrFalse = _str;}
    void setTrueString(const QString _str){mStrTrue = _str;}
};


#endif // LCDATAFORMATBOOL_H

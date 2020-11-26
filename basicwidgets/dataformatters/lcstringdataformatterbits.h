#ifndef LCSTRINGDATAFORMATTERBITS_H
#define LCSTRINGDATAFORMATTERBITS_H

#include "lcstringdataformatterbase.h"

class LCStringDataFormatterBits : public LCStringDataFormatterBase
{
private:
    //Валидатор вводимых данных.
    class CValidator : public QValidator
    {
    public:
        int      mSize;
        QChar    mSeparator;

        explicit CValidator(
                int _size = 0, 
                QChar _separator = QChar(), 
                QObject *_parent = nullptr);
        virtual ~CValidator(){}

        virtual QValidator::State validate(
                QString &_input, 
                int& _pos) const override;
    };

private:
    QChar       mFillCharUndef;   //Заполнение при отсутствии значения.
    QChar       mFillCharWrong;   //Заполнение при ошибочном значении.
    CValidator  mValidator;      //Валидатор.
public:

    explicit LCStringDataFormatterBits(    
                        int     _size           = 0, 
                        QChar   _separator      = QChar(),
                        QChar   _fillCharUndef  = msFillCharUndefDef,
                        QChar   _fillCharWrong  = msFillCharWrongDef);

    explicit LCStringDataFormatterBits( 
            const LCStringDataFormatterBits& _formatter);
    virtual ~LCStringDataFormatterBits();

    LCStringDataFormatterBits& operator=(
            const LCStringDataFormatterBits& _formatter);

    virtual QString     toString(const QByteArray& _data) override;
    virtual QString     normalizeString(const QString& _str) override;
    virtual QByteArray  toBytes(const QString& _str) override;
    virtual QString     undefStateString() override;
    virtual QValidator* validator() override {return &mValidator;}

    void setFillCharUndef(QChar _c){mFillCharUndef = _c;}
    void setFillCharWrong(QChar _c){mFillCharWrong = _c;}
    void setSize(int _size);
    void setSeparator(QChar _separator);
};

#endif // LCSTRINGDATAFORMATTERBITS_H



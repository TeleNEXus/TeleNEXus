#ifndef LCSTRINGDATAFORMATTERBITFIELD_H 
#define LCSTRINGDATAFORMATTERBITFIELD_H 

#include "lcstringdataformatterbase.h"

class LCStringDataFormatterBitfield : public LCStringDataFormatterBase
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
    CValidator  mValidator;       //Валидатор.
public:

    explicit LCStringDataFormatterBitfield(
        int _size = 0, 
        QChar _separator = QChar());

    explicit LCStringDataFormatterBitfield( 
            const LCStringDataFormatterBitfield& _formatter);

    virtual ~LCStringDataFormatterBitfield();

    LCStringDataFormatterBitfield& operator=(
            const LCStringDataFormatterBitfield& _formatter);

    virtual QString     toString(const QByteArray& _data) override;
    virtual QString     normalizeString(const QString& _str) override;
    virtual QByteArray  toBytes(const QString& _str) override;
    virtual QValidator* validator() override {return &mValidator;}

    void setSize(int _size);
    void setSeparator(QChar _separator);
};


#endif // LCSTRINGDATAFORMATTERBITFIELD_H

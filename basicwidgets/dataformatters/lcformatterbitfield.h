#ifndef LCSTRINGDATAFORMATTERBITFIELD_H 
#define LCSTRINGDATAFORMATTERBITFIELD_H 

#include "lcformatterbase.h"
#include <QValidator>

class LCFormatterBitfield : public LCFormatterBase
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

    explicit LCFormatterBitfield(
        int _size = 0, 
        QChar _separator = QChar());

    explicit LCFormatterBitfield( 
            const LCFormatterBitfield& _formatter);

    virtual ~LCFormatterBitfield();

    LCFormatterBitfield& operator=(
            const LCFormatterBitfield& _formatter);

    virtual QString     toString(const QByteArray& _data) override;
    virtual QString     normalize(const QString& _str) override;
    virtual QByteArray  toBytes(const QString& _str) override;
    virtual QValidator* validator() override {return &mValidator;}

    void setSize(int _size);
    void setSeparator(QChar _separator);
};


#endif // LCSTRINGDATAFORMATTERBITFIELD_H

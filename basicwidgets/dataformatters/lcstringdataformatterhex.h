#ifndef LCSTRINGDATAFORMATTERHEX_H
#define LCSTRINGDATAFORMATTERHEX_H

#include "lcstringdataformatterbase.h"

class LCStringDataFormatterHex : public LCStringDataFormatterBase
{
private:
    //Валидатор вводимых данных.
    class CValidator : public QValidator
    {
    public:
        const int&      mSize;
        const QChar&    mSeparator;

        explicit CValidator(const int& _size, const QChar& _separator, QObject *_parent = nullptr);
        ~CValidator(){}
        virtual QValidator::State validate(QString &_input, int& _pos) const override;
    };

private:
    int         mSize;              //Заданный размер последовательности в байтах.
    QChar       mSeparator;         //Разделитель между отображаемыми числами.
    QChar       mFillCharUndef;     //Заполнение при отсутствии значения.
    QChar       mFillCharWrong;     //Заполнение при ошибочном значении.
    CValidator* mpValidator;        //Валидатор.
public:

    explicit LCStringDataFormatterHex(    
                        int     _size           = 0, 
                        QChar   _separator      = QChar(),
                        QChar   _fillCharUndef  = msFillCharUndefDef,
                        QChar   _fillCharWrong  = msFillCharWrongDef);

    explicit LCStringDataFormatterHex( const LCStringDataFormatterHex& _formatter);
    virtual ~LCStringDataFormatterHex();

    LCStringDataFormatterHex& operator=(const LCStringDataFormatterHex& _formatter);

    virtual QString     toString(const QByteArray& _data) override;
    virtual QString     normalizeString(const QString& _str) override;
    virtual QByteArray  toBytes(const QString& _str) override;
    virtual QString     undefStateString() override;
    virtual QValidator* validator() override {return mpValidator;}

    void setFillCharUndef(QChar _c){mFillCharUndef = _c;}
    void setFillCharWrong(QChar _c){mFillCharWrong = _c;}
    void setSize(int _size);
    void setSeparator(QChar _separator);
};


#endif // LCSTRINGDATAFORMATTERHEX_H

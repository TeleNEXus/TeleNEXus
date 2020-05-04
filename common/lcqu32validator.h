#ifndef LCQUINT32VALIDATOR_H
#define LCQUINT32VALIDATOR_H

#include <QValidator>

class LCQValidatorU32 : public QValidator
{
    Q_OBJECT

    using MTUint32 = quint32;
    MTUint32 mMin;
    MTUint32 mMax;
public:
    explicit LCQValidatorU32(QObject *parent = nullptr);
    virtual ~LCQValidatorU32();
    virtual QValidator::State validate(QString &input, int &pos) const override;
    void setRange(MTUint32 _min, MTUint32 _max);

};

#endif // LCQUINT32VALIDATOR_H

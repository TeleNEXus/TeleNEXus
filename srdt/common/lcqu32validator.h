#ifndef LCQUINT32VALIDATOR_H
#define LCQUINT32VALIDATOR_H

#include <QValidator>
#include <ltypedef.h>

class LCQValidatorU32 : public QValidator
{
    Q_OBJECT
    using MTUint32 = lstd::U32;
    MTUint32 mMin;
    MTUint32 mMax;
public:
    explicit LCQValidatorU32(QObject *parent = nullptr);
    explicit LCQValidatorU32(MTUint32 min, MTUint32 max, QObject *parent = nullptr);

    virtual QValidator::State validate(QString &input, int &pos) const override;
    void setRange(MTUint32 _min, MTUint32 _max);
signals:

};

#endif // LCQUINT32VALIDATOR_H

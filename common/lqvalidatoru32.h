#ifndef LQVALIDATORU32_H
#define LQVALIDATORU32_H

#include <QValidator>

class LQValidatorU32 : public QValidator
{
    Q_OBJECT

    using MTUint32 = quint32;
    MTUint32 mMin;
    MTUint32 mMax;
public:
    explicit LQValidatorU32(QObject *parent = nullptr);
    virtual ~LQValidatorU32();
    virtual QValidator::State validate(QString &input, int &pos) const override;
    void setRange(MTUint32 _min, MTUint32 _max);

};

#endif // LQVALIDATORU32_H

#include "lqvalidatoru32.h"

LQValidatorU32::LQValidatorU32(QObject *parent) : QValidator(parent)
{
    mMin = 0;
    mMax = std::numeric_limits<MTUint32>::max();
}

LQValidatorU32::~LQValidatorU32()
{

}

LQValidatorU32::State LQValidatorU32::validate(QString &input, int &pos) const
{
    Q_UNUSED(pos);
    if(input.isEmpty()) return State::Intermediate;
    bool ok = false;
    MTUint32 res = input.toUInt(&ok);
    if(!ok) return State::Invalid;
    return ((res <= mMax) && (res >= mMin)) ? (State::Acceptable) : (State::Invalid);
}

void LQValidatorU32::setRange(MTUint32 _min, MTUint32 _max)
{
    mMin = _min;
    mMax = _max;
}

/* 
 * TeleNEXus is a simple SCADA programm
 *
 * Copyright (C) 2020 Sergey S. Kuzmenko
 *
 * This file is part of TeleNEXus.
 *
 * TeleNEXus is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * TeleNEXus is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with TeleNEXus.  If not, see <https://www.gnu.org/licenses/>.
 */
#include "lqvalidatoru32.h"

LQValidatorU32::LQValidatorU32(QObject *parent) : QValidator(parent)
{
  mMin = 0;
  mMax = std::numeric_limits<quint32>::max();
}

LQValidatorU32::~LQValidatorU32()
{
}

LQValidatorU32::State LQValidatorU32::validate(QString &input, int &pos) const
{
  Q_UNUSED(pos);
  if(input.isEmpty()) return State::Intermediate;
  bool ok = false;
  quint32 res = input.toUInt(&ok);
  if(!ok) return State::Invalid;
  return ((res <= mMax) && (res >= mMin)) ? 
    (State::Acceptable) : (State::Invalid);
}

void LQValidatorU32::setRange(quint32 _min, quint32 _max)
{
  mMin = _min;
  mMax = _max;
}

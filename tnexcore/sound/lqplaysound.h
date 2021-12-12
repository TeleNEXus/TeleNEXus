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
#ifndef LQPLAYSOUND_H_
#define LQPLAYSOUND_H_

#include <QObject>
#include <QBuffer>

class QIODevice;
class QAudioOutput;

class LQPlaySound : public QObject
{
public:
  enum ELoop{ Infinite = -1 };

private:
  /* QIODevice* mpDevice; */
  QByteArray mAudioData;
  QBuffer    mBuffer;
  
  int mLoops;
  int mLoopsCounter;
  QAudioOutput* mpAOut;
  QString mErrorMessage;

public:
  /* LQPlaySound() = delete; */
  explicit LQPlaySound(QObject *parent = nullptr);

  virtual ~LQPlaySound();

  /* QIODevice* device() const {return mpDevice;} */

  bool isFinished() const;
  int loops() const { return mLoops; }
  int loopsRemaining() const;
  void setLoops(int _number);
  void play();
  void stop();

  bool setDevice(QIODevice* _device);

  QString getErrorMessage(){ return mErrorMessage; }


private:
  void startPrivate();
};

#endif

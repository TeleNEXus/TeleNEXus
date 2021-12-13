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
#include "lqplaysound.h"
#include <QMediaPlayer>
#include <QDebug>


LQPlaySound::LQPlaySound(QObject *parent) :
  QObject(parent)
  ,mpPlayer(new QMediaPlayer(this))
  ,mStopFlag(false)
  ,mLoops(0)
  ,mLoopsCounter(0)
{
}

LQPlaySound::~LQPlaySound()
{
}


int LQPlaySound::loopsRemaining() const 
{
  if(mLoops == ELoop::Infinite) return ELoop::Infinite;
  return mLoopsCounter;
}

void LQPlaySound::setLoops(int _number)
{
  if(_number < 0) 
  {
    mLoops = ELoop::Infinite;
    mLoopsCounter = 0;
  }
  else 
  {
    mLoops = _number;
    mLoopsCounter = mLoops;
  }
}

void LQPlaySound::play()
{
  mStopFlag = false;
  mLoopsCounter = mLoops;
  mpPlayer->stop();
  mpPlayer->play();
}

void LQPlaySound::stop()
{
  mStopFlag = true;
  mpPlayer->stop();
  mLoopsCounter = 0;
}



bool LQPlaySound::setDevice(QIODevice* _device)
{

  if(!_device->isOpen()) 
  {
    mErrorMessage = "IO device of sound data is not open.";
    return false;
  }

  //Read audio file.
  mAudioData = _device->readAll();

  if(mAudioData.isNull()) 
  {
    mErrorMessage = "Can't read audio data from io device.";
    return false;
  }

  stop();

  if(mBuffer.isOpen())
  {
    mBuffer.close();
  }
  mBuffer.setBuffer(&mAudioData);
  mBuffer.open(QBuffer::ReadOnly);
  mBuffer.seek(0);

  mpPlayer->setMedia(0, &mBuffer);


  QObject::connect(mpPlayer, &QMediaPlayer::stateChanged,
      [this](QMediaPlayer::State _state)
      {
        switch(_state)
        {
        case QMediaPlayer::State::StoppedState:
            if(mStopFlag)
            {
              mLoopsCounter = 0;
              break;
            }

            if(mLoops == static_cast<int>(ELoop::Infinite))
            {
              mpPlayer->play();
              break;
            } 

            mLoopsCounter--;
            if(mLoopsCounter > 0)
            {
              mpPlayer->play();
            }
            else
            {
              mLoopsCounter = 0;
            }
            break;

        default:
          break;
        }
      });
  return true;
}

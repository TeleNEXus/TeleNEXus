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
#include <QAudioOutput>
#include <qaudio.h>
#include <QDebug>

LQPlaySound::LQPlaySound(QObject *parent) :
  QObject(parent)
  ,mpDevice(nullptr)
  ,mLoops(0)
  ,mLoopsCounter(0)
  ,mpAOut(nullptr)
{
}

LQPlaySound::~LQPlaySound()
{
  if(mpAOut == nullptr) return;
  mpAOut->stop();
  mpAOut->deleteLater();
}


bool LQPlaySound::isFinished() const
{ 
  if(mpAOut == nullptr) return true;
  return  (mpAOut->state() == QAudio::State::StoppedState) ? (true) : (false);
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
  if(mpAOut == nullptr) return;
  if(mpAOut->error() != QAudio::NoError) return;
  startPrivate();
  mLoopsCounter = mLoops;
}

void LQPlaySound::stop()
{
  if(mpAOut == nullptr) return;
  if(mpAOut->error() != QAudio::NoError) return;
  mpAOut->stop();
  mLoopsCounter = 0;
}

void LQPlaySound::startPrivate()
{
  if(mpAOut->error() != QAudio::NoError) return;
  mpDevice->reset();
  mpDevice->seek(44);
  mpAOut->start(mpDevice);
}


bool LQPlaySound::setDevice(QIODevice* _device)
{
  if(mpAOut != nullptr)
  {
    mpAOut->stop();
    mpAOut->deleteLater();
    mpAOut = nullptr;
  }

  mpDevice = _device;
  if(!mpDevice->isOpen()) 
  {
    mErrorMessage = "Sound data device is not open.";
    return false;
  }

  //Reading header of WAVE sound file.
  QByteArray wav_header_data = mpDevice->read(44);

  if(wav_header_data.isNull()) 
  {
    mErrorMessage = "Can't read WAVE header from device.";
    return false;
  }

  QString descr_riff = QString::fromLatin1(wav_header_data.data(), 4);
  QString descr_wave = QString::fromLatin1(&wav_header_data.data()[8], 4);

  if((descr_riff != QString("RIFF")) || (descr_wave != QString("WAVE"))) 
  {
    mErrorMessage = "Wrong WAVE header readed from device.";
    return false;
  }

  quint16 num_channels = *((quint16*)(&(wav_header_data.data()[22])));
  quint32 sample_rate = *((quint32*)(&(wav_header_data.data()[24])));
  /* quint32 byte_rate = *((quint32*)(&(data.data()[28]))); */
  quint16 bits_per_sample = *((quint16*)(&(wav_header_data.data()[34])));

  QAudioFormat format;



  format.setChannelCount(2);
  format.setSampleRate(44100);
  format.setSampleSize(16);
  format.setCodec("audio/pcm");
  format.setByteOrder(QAudioFormat::LittleEndian);
  format.setSampleType(QAudioFormat::SignedInt);

  /* format.setChannelCount(num_channels); */
  /* format.setSampleRate(sample_rate); */
  /* format.setSampleSize(bits_per_sample); */
  /* format.setCodec("audio/pcm"); */
  /* format.setByteOrder(QAudioFormat::LittleEndian); */
  /* format.setSampleType(QAudioFormat::SignedInt); */


  mpAOut = new QAudioOutput(format);

  QObject::connect(mpAOut, &QAudioOutput::stateChanged,
      [this](QAudio::State _state)
      {
        switch(_state)
        {
        case QAudio::State::IdleState:

          if(mLoops == static_cast<int>(ELoop::Infinite))
          {
            startPrivate();
            break;
          } 

          if(mLoops == 0) 
          {
            if(mpAOut->error() != QAudio::NoError) return;
            mpAOut->stop();
            break;
          }

          mLoopsCounter--;
          if(mLoopsCounter >= 0)
          {
            startPrivate();
          }
          else
          {
            mLoopsCounter = 0;
            if(mpAOut->error() != QAudio::NoError) return;
            mpAOut->stop();
          }
          break;

        default:
          break;
        }
      });
  return true;
}

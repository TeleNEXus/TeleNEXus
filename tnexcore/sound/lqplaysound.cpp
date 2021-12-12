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

/* //------------------------------------------------------------------------------ */
/* void LQPlaySound::LCAudioBuffer::setAudioOutput(QAudioOutput* _out) */
/* { */
/*   mpOut = _out; */
/* } */

/* //------------------------------------------------------------------------------ */
/* bool	LQPlaySound::LCAudioBuffer::atEnd() const */
/* { */
/*   qDebug() << QStringLiteral("atEnd()"); */
/*   return QBuffer::atEnd(); */
/* } */

/* //------------------------------------------------------------------------------ */
/* qint64	LQPlaySound::LCAudioBuffer::pos() const */
/* { */
/*   qint64 pos = QBuffer::pos() + mAddPos; */ 
/*   return pos; */ 
/* } */

/* //------------------------------------------------------------------------------ */
/* bool	LQPlaySound::LCAudioBuffer::seek(qint64 pos) */
/* { */
/*   mAddPos = 0; */
/*   mStopFlag = false; */
/*   return QBuffer::seek(pos); */
/* } */

/* //------------------------------------------------------------------------------ */
/* qint64	LQPlaySound::LCAudioBuffer::readData(char *data, qint64 len) */
/* { */
/*   qDebug() << QString("readData(len = %1)").arg(len); */

/*   if(mStopFlag) */
/*   { */
/*     memset(data, 0, len); */

/*     if(mpOut != nullptr) */
/*     { */
/*       /1* mpOut->stop(); *1/ */
/*       mpOut->suspend(); */
/*     } */

/*     QBuffer::seek(0); */
/*     mStopFlag = false; */
/*     mAddPos += len; */
/*     return len; */
/*   } */

/*   qint64 rl = QBuffer::readData(data, len); */

/*   if(rl < len) */
/*   { */
/*     mStopFlag = true; */
/*     mAddPos += len - rl; */
/*     memset(&data[rl], 0, len - rl); */
/*   } */
/*   return len; */
/* } */






LQPlaySound::LQPlaySound(QObject *parent) :
  QObject(parent)
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
  /* if(mpAOut->error() != QAudio::NoError) return; */
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
  mpAOut->stop();
  mpAOut->reset();
  mBuffer.reset();
  mpAOut->start(&mBuffer);
}


bool LQPlaySound::setDevice(QIODevice* _device)
{
  if(mpAOut != nullptr)
  {
    mpAOut->stop();
    mpAOut->deleteLater();
    mpAOut = nullptr;
  }

  if(!_device->isOpen()) 
  {
    mErrorMessage = "Sound data device is not open.";
    return false;
  }



  //Reading of WAVE sound file.
  mAudioData = _device->readAll();

  if(mAudioData.isNull()) 
  {
    mErrorMessage = "Can't read WAVE header from device.";
    return false;
  }

  QString descr_riff = QString::fromLatin1(mAudioData.data(), 4);
  QString descr_wave = QString::fromLatin1(&mAudioData.data()[8], 4);

  if((descr_riff != QString("RIFF")) || (descr_wave != QString("WAVE"))) 
  {
    mErrorMessage = "Wrong WAVE header readed from device.";
    return false;
  }

  quint16 num_channels = *((quint16*)(&(mAudioData.data()[22])));
  quint32 sample_rate = *((quint32*)(&(mAudioData.data()[24])));
  /* quint32 byte_rate = *((quint32*)(&(data.data()[28]))); */
  quint16 bits_per_sample = *((quint16*)(&(mAudioData.data()[34])));

  QAudioFormat format;


  format.setChannelCount(num_channels);
  format.setSampleRate(sample_rate);
  format.setSampleSize(bits_per_sample);
  format.setCodec("audio/pcm");
  format.setByteOrder(QAudioFormat::LittleEndian);
  format.setSampleType(QAudioFormat::SignedInt);

  mpAOut = new QAudioOutput(format);

  mAudioData = mAudioData.mid(44);
  mAudioData.resize(mAudioData.size() - (mAudioData.size()%4));
  mBuffer.setData(mAudioData);
  mBuffer.open(QBuffer::ReadOnly);
  /* mBuffer.setAudioOutput(mpAOut); */


  QObject::connect(mpAOut, &QAudioOutput::stateChanged,
      [this](QAudio::State _state)
      {
        switch(_state)
        {
        case QAudio::State::IdleState:
        /* case QAudio::State::StoppedState: */
            /* mpAOut->stop(); */
            /* break; */

          mpAOut->reset();
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

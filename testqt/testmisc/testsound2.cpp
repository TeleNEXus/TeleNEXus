#include <QApplication>
#include <QtWidgets>
#include <qaudio.h>
#include <qglobal.h>
#include <qnamespace.h>
#include <QScrollArea>
#include <qscrollarea.h>
#include <QComboBox>
#include <QVBoxLayout>
#include <QPixmap>
#include <QMovie>
#include <QPushButton>
#include <QDebug>
#include <QTouchDevice>
#include <QList>
#include <QMouseEvent>
#include <QLineEdit>
#include <QAudioOutput>
#include <QAudioDeviceInfo>
#include <QBuffer>
#include <QAudioBuffer>
#include <string>
#include <functional>


class LCAudioBuffer : public QBuffer
{
private:
  qint64 mAddPos = 0;
  bool mStopFlag = false;
  QAudioOutput* mpOut = nullptr;
  
public:
  using QBuffer::QBuffer;

  void setAudioAutput(QAudioOutput* _out)
  {
    mpOut = _out;
  }

  virtual bool	atEnd() const override
  {
    qDebug() << QStringLiteral("atEnd()");
    return QBuffer::atEnd();
  }





  /* virtual bool	canReadLine() const override */
  /* virtual void	close() override */
  /* virtual bool	open(QIODevice::OpenMode flags) override */

  virtual qint64	pos() const override
  {
    /* qint64 pos = QBuffer::pos() + mAddPos; */ 
    qint64 pos = QBuffer::pos();
    qDebug() << QStringLiteral("pos() = ") << pos;
    return pos; 
    /* return QBuffer::pos(); */
  }

  virtual bool	seek(qint64 pos) override
  {
    qDebug() << QStringLiteral("seek() = ") << pos;
    /* mAddPos = 0; */
    /* mStopFlag = false; */

    /* if(pos > size()) */
    /* { */
    /*   pos = size(); */
    /* } */
    return QBuffer::seek(pos);
  }

  /* virtual qint64	size() const override */

  virtual qint64	readData(char *data, qint64 len) override
  {
    qDebug() << QString("readData(len = %1)").arg(len);

    qint64 rl = QBuffer::readData(data, len);

    if(rl < len)
    {
      
      QBuffer::reset();
      QBuffer::readData(&(data[rl]), len - rl);
    }

    return len;



    /* qDebug() << QString("readData(len = %1)").arg(len); */

    /* if(mStopFlag) */
    /* { */
    /*   memset(data, 0, len); */

    /*   /1* if(data != nullptr) *1/ */
    /*   /1* { *1/ */
    /*   /1*   for(qint64 i = 0; i < len; i++) *1/ */
    /*   /1*   { *1/ */
    /*   /1*     data[0] = 0; *1/ */
    /*   /1*   } *1/ */
    /*   /1* } *1/ */

    /*   if(mpOut != nullptr) */
    /*   { */
    /*     mpOut->stop(); */
    /*   } */
    /*   QBuffer::seek(0); */
    /*   mAddPos = 0; */


    /*   mStopFlag = false; */
    /*   /1* mAddPos += len; *1/ */
    /*   return len; */
    /* } */

    /* qint64 rl = QBuffer::readData(data, len); */

    /* if(rl < len) */
    /* { */
    /*   qDebug() << "rl = " << rl; */
    /*   mStopFlag = true; */
    /*   mAddPos += len - rl; */
    /*   memset(&data[rl], 0, len - rl); */
    /* } */

    /* return len; */
  }
  /* virtual qint64	writeData(const char *data, qint64 len) override */

};

class LQPlaySound : QObject
{
public:
  enum ELoop{ Infinite = -1 };
private:

  LCAudioBuffer mBuffer;
  /* QBuffer mBuffer; */
  QByteArray mAudioData;

  QIODevice* mpDevice;
  int mLoops;
  int mLoopsCounter;
  QAudioOutput* mpAOut;
public:
  LQPlaySound() = delete;
  explicit LQPlaySound(QIODevice* _device, QObject *parent = nullptr) :
    QObject(parent)
    ,mpDevice(_device)
    ,mLoops(0)
    ,mLoopsCounter(0)
    ,mpAOut(nullptr)
  {

    if(!mpDevice->isOpen()) return;

    mAudioData = mpDevice->readAll();
    mpDevice->close();

    if(mAudioData.isNull()) 
    {
      return;
    }


    QString descr_riff = QString::fromLatin1(mAudioData.data(), 4);
    QString descr_wave = QString::fromLatin1(&mAudioData.data()[8], 4);

    if(descr_riff != QString("RIFF"))  return;
    if(descr_wave != QString("WAVE"))  return;

    quint32 chunk_size =  *((quint32*)(&(mAudioData.data()[4])));
    quint16 num_channels = *((quint16*)(&(mAudioData.data()[22])));
    quint32 sample_rate = *((quint32*)(&(mAudioData.data()[24])));
    quint32 byte_rate = *((quint32*)(&(mAudioData.data()[28])));
    quint16 bits_per_sample = *((quint16*)(&(mAudioData.data()[34])));

    QAudioFormat format;



    qDebug() << "chunk_size       = " << chunk_size;
    qDebug() << "num_channels     = " << num_channels;
    qDebug() << "sample_rate      = " << sample_rate;
    qDebug() << "byte_rate        = " << byte_rate;
    qDebug() << "bits_per_sample  = " << bits_per_sample;

    /* format.setChannelCount(2); */
    /* format.setSampleRate(44100); */
    /* format.setSampleSize(16); */
    /* format.setCodec("audio/pcm"); */
    /* format.setByteOrder(QAudioFormat::LittleEndian); */
    /* format.setSampleType(QAudioFormat::SignedInt); */

    format.setChannelCount(num_channels);
    format.setSampleRate(sample_rate);
    format.setSampleSize(bits_per_sample);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::SignedInt);

    qDebug() << "Audio data full size = " << mAudioData.size();
    mAudioData = mAudioData.mid(44);
    qDebug() << "Audio data size = " << mAudioData.size();
    mAudioData.resize(mAudioData.size() - (mAudioData.size()%4));
    qDebug() << "Audio data align size = " << mAudioData.size();


    mBuffer.setData(mAudioData);
    mBuffer.open(QIODevice::ReadOnly);

    mpAOut = new QAudioOutput(format);

    mBuffer.setAudioAutput(mpAOut);

    QObject::connect(mpAOut, &QAudioOutput::stateChanged,
        [this](QAudio::State _state)
        {
          qDebug() << "Audio out state = " << _state;
          switch(_state)
          {

          /* case QAudio::State::IdleState: */
          /*   mpAOut->stop(); */
          /*   mBuffer.reset(); */
          /*   break; */

          case QAudio::State::StoppedState:

            mpAOut->reset();

            if(mLoops == static_cast<int>(ELoop::Infinite))
            {
              startPrivate();
              break;
            } 

            if(mLoops == 0) 
            {
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
              mpAOut->stop();
            }
            break;

          case QAudio::State::SuspendedState:
          case QAudio::State::IdleState:
          case QAudio::State::InterruptedState:
            mpAOut->stop();
            mpAOut->reset();
            mBuffer.reset();
            break;

          default:
            
            break;
          }

        });

  }

  virtual ~LQPlaySound()
  {
    if(mpAOut != nullptr) return;
    mpAOut->stop();
    mpAOut->deleteLater();
  }

  QIODevice* device() const {return mpDevice;}

  bool isFinished() const
  { 
    if(mpAOut == nullptr) return true;
    return  (mpAOut->state() == QAudio::State::StoppedState) ? (true) : (false);
  }
  int loops() const { return mLoops; }
  int loopsRemaining() const 
  {
    if(mLoops == ELoop::Infinite) return ELoop::Infinite;
    return mLoopsCounter;
  }

  void setLoops(int _number)
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

  void play()
  {
    if(mpAOut == nullptr) return;
    startPrivate();
    mLoopsCounter = mLoops;
  }

  void stop()
  {
    if(mpAOut == nullptr) return;
    mpAOut->stop();
    mLoopsCounter = 0;
  }

private:
  void startPrivate()
  {
    qDebug() << mpAOut->error();
    /* mpAOut->reset(); */
    /* mpDevice->seek(44); */
    mBuffer.seek(0);
    mBuffer.reset();
    /* mpAOut->start(&mBuffer); */
    /* mpAOut->reset(); */
    mpAOut->stop();
    if(mpAOut->state() == QAudio::State::IdleState)
    {
      mpAOut->resume();
    }
    else
    {
      mpAOut->start(&mBuffer);
    }
  }
};


int main(int argc, char** argv)
{
  QApplication app(argc, argv);

  QFrame* frame = new QFrame;
  QPushButton* button_play = new QPushButton("Play");
  QPushButton* button_play_loop = new QPushButton("Play loop");
  QPushButton* button_stop = new QPushButton("Stop");
  QLineEdit* loops_edit = new QLineEdit();
  QVBoxLayout* layout = new QVBoxLayout;

  if(QAudioDeviceInfo::defaultOutputDevice().isNull())
  {
    qDebug() << "Can't find default audio device.";
  }
  else
  {
    qDebug() << QAudioDeviceInfo::defaultOutputDevice().deviceName();
  }

  qDebug().noquote() << "Audio devices info: ";
  auto avail = QAudioDeviceInfo::availableDevices(QAudio::Mode::AudioOutput);
  

  for(int i = 0; i < avail.size(); i++)
  {
    qDebug().noquote() << QString("[%1] : %2").arg(i).arg(avail[i].deviceName());
  }



/* QAudioOutput(pulseaudio): pa_stream_write, error = Недопустимый параметр */
                      

  /* QFile sound_file("../k2.wav"); */
  /* QFile sound_file("../k1.wav"); */
  /* QFile sound_file("../sirena1.wav"); */
  QFile sound_file("../sirena_001.wav");
  /* QFile sound_file("../discord-sounds.wav"); */
  /* QFile sound_file("../sirena_003.wav"); */
  /* QFile sound_file("../beep-07a.wav"); */
              /* QFile sound_file("/home/serg/D_DRIVE/music/knopka-klik-blizkii-myagkii-priglushennyii.wav"); */
  /* QFile sound_file("/home/serg/D_DRIVE/music/knopka-klik-vyisokii-myagkii-blizkii-nejnyii.wav"); */

  /* QFile sound_file("/home/serg/D_DRIVE/music/knopka-klik-vyisokii-rezkii-blizkii.wav"); */
  /* QFile sound_file("/home/serg/D_DRIVE/music/knopka-klik-vyisokii-zvonkii-blizkii-gromkii.wav"); */
  /* QFile sound_file("/home/serg/D_DRIVE/music/sirena-korabelnaya-trevoga-na-korable-26750.wav"); */
  /* QFile sound_file("/home/serg/D_DRIVE/music/beep-07a.wav"); */

  if(!sound_file.open(QFile::OpenModeFlag::ReadOnly)) 
  {
    qDebug() << "Can't open sound file.";
  }

  LQPlaySound* play_sound = new LQPlaySound(&sound_file);

  qDebug() << "Play loops = " << play_sound->loops();

  QObject::connect(loops_edit, &QLineEdit::editingFinished,
      [play_sound, loops_edit]()
      {
        if(loops_edit->text().isNull()) return;
        if(loops_edit->text() == "") return;
        if(loops_edit->text().toLower() == "inf")
        {
          play_sound->setLoops(LQPlaySound::Infinite);
          qDebug() << "Set inf";
          return;
        }
        bool flag = false;
        int loops = loops_edit->text().toInt(&flag);
        if(flag)
        {
          play_sound->setLoops(loops);
        }
        else
        {
          loops_edit->setText(QString("%1").arg(play_sound->loops()));
        }
      });

  QObject::connect(button_play, &QPushButton::pressed,
      [play_sound]()
      {
        qDebug() << "-----------------Play sound";
        play_sound->play();
      });

  QObject::connect(button_play_loop, &QPushButton::pressed,
      []()
      {
      });

  QObject::connect(button_stop, &QPushButton::pressed,
      [play_sound]()
      {
        qDebug() << "-----------------Stop sound";
        play_sound->stop();
      });


  layout->addWidget(loops_edit);
  layout->addWidget(button_play);
  layout->addWidget(button_play_loop);
  layout->addWidget(button_stop);
  frame->setLayout(layout);
  frame->show();
  return app.exec();
}

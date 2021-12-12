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

    return QBuffer::seek(pos);
  }

  /* virtual qint64	size() const override */

  virtual qint64	readData(char *data, qint64 len) override
  {
    qDebug() << QString("readData(len = %1)").arg(len);

    if(mStopFlag)
    {
      memset(data, 0, len);

      if(mpOut != nullptr)
      {
        mpOut->stop();
      }
      QBuffer::seek(0);
      /* mAddPos = 0; */
      mStopFlag = false;
      mAddPos += len;
      return len;
    }

    qint64 rl = QBuffer::readData(data, len);

    if(rl < len)
    {
      qDebug() << "rl = " << rl;
      mStopFlag = true;
      mAddPos += len - rl;
      memset(&data[rl], 0, len - rl);
    }

    return len;
  }
  /* virtual qint64	writeData(const char *data, qint64 len) override */

};

class LQPlaySound : QObject
{
public:
  enum ELoop{ Infinite = -1 };
private:

  QAudioFormat mFormat;
  /* LCAudioBuffer mBuffer; */
  QBuffer mBuffer;
  QByteArray mAudioData;
  /* qint64 mPos = 0; */

  QIODevice* mpDevice;
  QIODevice* mpOut = nullptr;

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

    mFormat.setChannelCount(num_channels);
    mFormat.setSampleRate(sample_rate);
    mFormat.setSampleSize(bits_per_sample);
    mFormat.setCodec("audio/pcm");
    mFormat.setByteOrder(QAudioFormat::LittleEndian);
    mFormat.setSampleType(QAudioFormat::SignedInt);

    qDebug() << "Audio data full size = " << mAudioData.size();
    mAudioData = mAudioData.mid(44);
    qDebug() << "Audio data size = " << mAudioData.size();
    mAudioData.resize(mAudioData.size() - (mAudioData.size()%4));
    qDebug() << "Audio data align size = " << mAudioData.size();

    mBuffer.setAudioAutput(mpAOut);

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
    mpAOut = new QAudioOutput(mFormat);

    mpOut = mpAOut->start(); 

    if(mpOut ==  nullptr) qDebug() << "Wrong io";


    connect(mpOut, &QIODevice::bytesWritten,
        [](qint64 len)
        {
          qDebug() << "Signal bytes writen.";
          /* /1* mPos += len; *1/ */
          /* if(mPos >= mAudioData.size()) */
          /* { */
          /*   mPos = 0; */
          /* } */
          /* mpOut->write(&( mAudioData.data()[mPos] ), mAudioData.size()  - mPos); */
        });

    connect(mpAOut, &QAudioOutput::stateChanged,
        [](QAudio::State _state)
        {
          qDebug().noquote() << "State = " << _state;
        });



    mpOut->write(&(mAudioData.data()[0]), mAudioData.size());
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
                      

  QFile sound_file("../k2.wav");
  /* QFile sound_file("../k1.wav"); */
  /* QFile sound_file("../sirena1.wav"); */
  /* QFile sound_file("../sirena_001.wav"); */
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

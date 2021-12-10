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

#include <QAudioOutput>
#include <QAudioDeviceInfo>
#include <QBuffer>

#include <limits>





class LCPlaySound : QObject
{
public:
  enum ELoop{ Infinite = -1 };
private:
  QIODevice* mpDevice;
  int mLoops;
  int mLoopsCounter;
  QAudioOutput* mpAOut;
public:
  LCPlaySound() = delete;
  explicit LCPlaySound(QIODevice* _device, QObject *parent = nullptr) :
    QObject(parent)
    ,mpDevice(_device)
    ,mLoops(-1)
    ,mLoopsCounter(0)
    ,mpAOut(nullptr)
  {
    if(mpDevice == nullptr) return;
    if(!mpDevice->open(QIODevice::OpenModeFlag::ReadOnly))
    {
      return;
    }

    QByteArray data = mpDevice->read(44);

    if(data.isNull()) 
    {
      return;
    }


    QString descr_riff = QString::fromLatin1(data.data(), 4);
    QString descr_wave = QString::fromLatin1(&data.data()[8], 4);

    if(descr_riff != QString("RIFF"))  return;
    if(descr_wave != QString("WAVE"))  return;

    quint16 num_channels = *((quint16*)(&(data.data()[22])));
    quint32 sample_rate = *((quint32*)(&(data.data()[24])));
    /* quint32 byte_rate = *((quint32*)(&(data.data()[28]))); */
    quint16 bits_per_sample = *((quint16*)(&(data.data()[34])));

    QAudioFormat format;

    format.setChannelCount(num_channels);
    format.setSampleRate(sample_rate);
    format.setSampleSize(bits_per_sample);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::SignedInt);

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
            } 

            if(mLoops == 0) 
            {
              mpAOut->stop();
            }

            mLoopsCounter--;
            if(mLoopsCounter > 0)
            {
              startPrivate();
            }
            else
            {
              mpAOut->stop();
            }
            break;

          default:
            break;
          }

        });

  }

  virtual ~LCPlaySound() {}

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
    if(_number < 0) mLoops = ELoop::Infinite;
    else mLoops = _number;
  }

  void play()
  {
    if(mpAOut == nullptr) return;
    mLoopsCounter = mLoops;
    startPrivate();
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
    mpDevice->seek(44);
    mpAOut->start(mpDevice);
  }
};






















class CLoopBuffer: public QBuffer 
{
public:
  using QBuffer::QBuffer;

virtual bool	atEnd() const override
{
  qDebug() << "atEnd";
  return QBuffer::atEnd();
}

virtual bool	canReadLine() const override
{
  qDebug() << "canReadLine";
  return QBuffer::canReadLine();
}

/* virtual void	close() override; */

/* virtual bool	open(QIODevice::OpenMode flags) override; */

virtual qint64	pos() const override
{
  /* qDebug() << "pos"; */
  return QBuffer::pos();
}

virtual bool	seek(qint64 pos) override
{
  /* qDebug() << QString("seek(%1)").arg(pos); */
  return QBuffer::seek(pos);
}

protected:

virtual qint64	readData(char *data, qint64 len) override
{

  /* /1* qDebug() << QString("readData len = %1").arg(len); *1/ */

  /* /1* if(QBuffer::atEnd()) QBuffer::seek(0); *1/ */
  /* qint64 rl = QBuffer::readData(data, len); */
  /* qint64 rl2 = 0; */

  /* if(rl != len) */
  /* { */
  /*   QBuffer::seek(0); */
  /*   rl2 = QBuffer::readData(&data[rl+1], len-rl); */
  /* } */


  /* /1* if(QBuffer::atEnd()) QBuffer::seek(0); *1/ */

  /* return rl + rl2; */
  return QBuffer::readData(data, len);
}

};



static void test();

int main(int argc, char** argv)
{
  QApplication app(argc, argv);

  QFrame* frame = new QFrame;
  QPushButton* button_play = new QPushButton("Play");
  QPushButton* button_play_loop = new QPushButton("Play loop");
  QPushButton* button_stop = new QPushButton("Stop");
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
    qDebug().noquote() << avail[i].deviceName();
  }


  QFile hardfile("../sirena_001.wav");
  qDebug() << hardfile.open(QIODevice::ReadOnly);


  QByteArray data = hardfile.readAll();



  /* data.data()[] */



  /* file.seek(44); // Length of wav header */

  QAudioFormat format;


  QString descr = QString::fromLatin1(data.data(), 12);

  qDebug() << "Descriptor = " << descr;

  QString descr_riff = QString::fromLatin1(data.data(), 4);
  QString descr_wave = QString::fromLatin1(&data.data()[8], 4);

  if(descr_riff != QString("RIFF")) qDebug() << "Wrong RIFF descriptor";
  else qDebug() << "RIFF descriptro is OK";
  if(descr_wave != QString("WAVE")) qDebug() << "Wrong WAVE descriptor";
  else qDebug() << "WAVE descriptro is OK";



  quint16 num_channels = *((quint16*)(&(data.data()[22])));
  quint32 sample_rate = *((quint32*)(&(data.data()[24])));
  quint32 byte_rate = *((quint32*)(&(data.data()[28])));
  quint16 bits_per_sample = *((quint16*)(&(data.data()[34])));

  qDebug() << "Num channels = " << num_channels;
  qDebug() << "Sample rate = " << sample_rate;
  qDebug() << "Byte rate = " << byte_rate;
  qDebug() << "Bits per sample" << bits_per_sample;

  format.setSampleSize(16);

  format.setSampleRate(sample_rate);

  /* format.setSampleRate(44100); */
  /* format.setChannelCount(1);    // mono */
  format.setChannelCount(2); // stereo
  format.setCodec("audio/pcm");
  format.setByteOrder(QAudioFormat::LittleEndian);
  format.setSampleType(QAudioFormat::SignedInt);

  QAudioOutput* audioOutput = new QAudioOutput(format);

  audioOutput->setNotifyInterval(1);
  qDebug() << "Notify interval" << audioOutput->notifyInterval();


  /* data = data.mid(44); */
  /* data = data.mid(36); */
  /* QBuffer file(&data); */
  CLoopBuffer file(&data);

  file.open(QIODevice::OpenModeFlag::ReadOnly);



  QObject::connect(audioOutput, &QAudioOutput::stateChanged,
      [&file, audioOutput](QAudio::State _state)
      {
        qDebug() << _state;


        switch(_state)
        {
        case QAudio::State::IdleState:
          file.seek(44);
          audioOutput->start(&file);
          break;

        default:
          break;
        }

      });




  qDebug() << audioOutput->state();


  QObject::connect(button_play, &QPushButton::pressed,
      [audioOutput, &file]()
      {
        qDebug() << "-----------------Play sound";

        file.seek(44);
        audioOutput->start(&file);
      });

  QObject::connect(button_play_loop, &QPushButton::pressed,
      []()
      {
      });

  QObject::connect(button_stop, &QPushButton::pressed,
      [audioOutput]()
      {
        qDebug() << "-----------------Play sound";
        audioOutput->stop();
      });


  layout->addWidget(button_play);
  layout->addWidget(button_play_loop);
  layout->addWidget(button_stop);
  frame->setLayout(layout);
  frame->show();
  return app.exec();
}
//------------------------------------------------
static void test()
{ 

  static QFile file("../sirena1.wav");
  qDebug() << file.open(QIODevice::ReadOnly);
  file.seek(44); // Length of wav header

  static QAudioFormat format;

  format.setSampleSize(16);
  format.setSampleRate(44100);
  //format.setChannelCount(1);    // mono
  format.setChannelCount(2); // stereo
  format.setCodec("audio/pcm");
  format.setByteOrder(QAudioFormat::LittleEndian);
  format.setSampleType(QAudioFormat::SignedInt);

  static QAudioOutput audioOutput(format);
  audioOutput.start(&file);
  qDebug() << audioOutput.state();
}


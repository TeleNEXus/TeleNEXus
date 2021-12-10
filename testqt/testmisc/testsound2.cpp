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



  /* QFile file("../sirena1.wav"); */
  /* qDebug() << file.open(QIODevice::ReadOnly); */
  /* file.seek(44); // Length of wav header */

  /* QAudioFormat format; */

  /* format.setSampleSize(16); */
  /* format.setSampleRate(44100); */
  /* //format.setChannelCount(1);    // mono */
  /* format.setChannelCount(2); // stereo */
  /* format.setCodec("audio/pcm"); */
  /* format.setByteOrder(QAudioFormat::LittleEndian); */
  /* format.setSampleType(QAudioFormat::SignedInt); */

  /* QAudioOutput audioOutput(format); */
  /* audioOutput.start(&file); */
  /* qDebug() << audioOutput.state(); */





  QObject::connect(button_play, &QPushButton::pressed,
      []()
      {
        qDebug() << "-----------------Play sound";
        /* test(); */
      });

  QObject::connect(button_play_loop, &QPushButton::pressed,
      []()
      {
      });

  QObject::connect(button_stop, &QPushButton::pressed,
      []()
      {
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


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
#include <QMediaPlayer>


class LQPlaySound : QObject
{
public:
  enum ELoop{ Infinite = -1 };
private:

  QMediaPlayer* mpPlayer;

  QIODevice* mpDevice;
  int mLoops;
  int mLoopsCounter;
  bool mStopFlag = false;;

public:
  LQPlaySound() = delete;
  explicit LQPlaySound(QIODevice* _device, QObject *parent = nullptr) :
    QObject(parent)
    ,mpPlayer(new QMediaPlayer(this))
    ,mpDevice(_device)
    ,mLoops(0)
    ,mLoopsCounter(0)
  {
    if(!mpDevice->isOpen()) return;
    mpPlayer->setAudioRole(QAudio::Role::MusicRole);
    /* mpPlayer->setMedia(QMediaContent(), mpDevice); */
    mpPlayer->setMedia(0, mpDevice);


    connect(mpPlayer, &QMediaPlayer::stateChanged,
        [this](QMediaPlayer::State _state)
        {
          qDebug().noquote() << "Media player state = " << _state;

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
            if(mLoopsCounter >= 0)
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
  }

  virtual ~LQPlaySound()
  {
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
    }
  }

  void play()
  {
    mStopFlag = false;
    mLoopsCounter = mLoops;
    mpPlayer->stop();
    mpPlayer->play();
  }

  void stop()
  {
    mStopFlag = true;
    mpPlayer->stop();
  }

private:
  void startPrivate()
  {
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

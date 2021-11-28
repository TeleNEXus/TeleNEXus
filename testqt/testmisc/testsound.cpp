#include <QApplication>
#include <QtWidgets>
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

#include <QSound>

int main(int argc, char** argv)
{
  QApplication app(argc, argv);

  QFrame* frame = new QFrame;
  QPushButton* button_play = new QPushButton("Play");
  QPushButton* button_play_loop = new QPushButton("Play loop");
  QPushButton* button_stop = new QPushButton("Stop");
  QVBoxLayout* layout = new QVBoxLayout;

  QSound* sound = 
    /* new QSound("../../../../../Музыка/sirena_001.wav", frame); */
    new QSound("../beep-07a.wav", frame);


  QObject::connect(button_play, &QPushButton::pressed,
      [sound]()
      {
        qDebug() << "Play sound";
        sound->setLoops(0);
        sound->play();
      });

  QObject::connect(button_play_loop, &QPushButton::pressed,
      [sound]()
      {
        qDebug() << "Play sound";
        sound->setLoops(QSound::Loop::Infinite);
        sound->play();
      });

  QObject::connect(button_stop, &QPushButton::pressed,
      [sound]()
      {
        qDebug() << "Stop sound";
        sound->stop();
      });


  layout->addWidget(button_play);
  layout->addWidget(button_play_loop);
  layout->addWidget(button_stop);
  frame->setLayout(layout);
  frame->show();
  return app.exec();
}



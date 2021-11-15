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


//------------------------------------------------------------------------------
class CTouchEvent : public QObject
{
public: 
  explicit CTouchEvent(QObject* _parent = nullptr) : QObject(_parent)
  {
  }
  virtual bool eventFilter(QObject* _obj, QEvent* _event) override
  {


    static QMouseEvent mep(QEvent::MouseButtonPress, QPointF(0.0,0.0), 
        Qt::MouseButton::NoButton, 0, 0);

    static QMouseEvent mer(QEvent::MouseButtonRelease, QPointF(0.0, 0.0), 
        Qt::MouseButton::NoButton, 0, 0);

    bool ret = false;

    switch(_event->type())
    {

    case QEvent::Type::TouchBegin:
      qDebug() << "Event = " << _event->type();
      QApplication::sendEvent(_obj, &mep);
      ret = true;
      break;
    case QEvent::Type::TouchEnd:
      qDebug() << "Event = " << _event->type();
      QApplication::sendEvent(_obj, &mer);
      ret = true;
      break;
    case QEvent::Type::TouchUpdate:
      qDebug() << "Event = " << _event->type();
      break;
    case QEvent::Type::TouchCancel:
      qDebug() << "Event = " << _event->type();
      break;

    case QEvent::Type::MouseButtonPress:
      qDebug() << "Event = " << _event->type();
      break;

    case QEvent::Type::MouseButtonRelease:
      qDebug() << "Event = " << _event->type();
      break;

    default:
      break;
    }
    return ret;
  } 
};



class CButton : public QPushButton
{
public:
  explicit CButton(const QString& _text = QString()) : QPushButton(_text, nullptr)
  {
  }

  virtual bool event(QEvent* _event) override
  {

    bool ret = false;
    switch(_event->type())
    {

    case QEvent::Type::TouchBegin:
      qDebug() << "Event = " << _event->type();


      ret = true;
      break;
    case QEvent::Type::TouchEnd:
      qDebug() << "Event = " << _event->type();
      break;
    case QEvent::Type::TouchUpdate:
      qDebug() << "Event = " << _event->type();
      break;
    case QEvent::Type::TouchCancel:
      qDebug() << "Event = " << _event->type();
      break;

    case QEvent::Type::MouseButtonPress:
      qDebug() << "Event = " << _event->type();
      break;

    case QEvent::Type::MouseButtonRelease:
      qDebug() << "Event = " << _event->type();
      break;

    default:
      break;
    }
    QPushButton::event(_event);
    return ret;
  }

};

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    /* app.setAttribute(Qt::AA_SynthesizeMouseForUnhandledTouchEvents, true); */
    /* app.setAttribute(Qt::AA_SynthesizeTouchForUnhandledMouseEvents, false); */

    QFrame* frame = new QFrame();
    /* QPushButton* button1 = new CButton("Button 1"); */
    QPushButton* button1 = new QPushButton("Button 1");

    button1->installEventFilter(new CTouchEvent(button1));


    QPushButton* button2 = new QPushButton("Button 2");

    button1->setAttribute(Qt::WA_AcceptTouchEvents);
    button1->setAttribute(Qt::WA_TouchPadAcceptSingleTouchEvents);

    button2->setAttribute(Qt::WA_AcceptTouchEvents);
    button2->setAttribute(Qt::WA_TouchPadAcceptSingleTouchEvents);

    QVBoxLayout* layout = new QVBoxLayout();

    layout->addWidget(button1);
    layout->addWidget(button2);

    frame->setLayout(layout);

    button1->setAttribute(Qt::WA_AcceptTouchEvents);
    button1->setAttribute(Qt::WA_TouchPadAcceptSingleTouchEvents);

    button2->setAttribute(Qt::WA_AcceptTouchEvents);
    button2->setAttribute(Qt::WA_TouchPadAcceptSingleTouchEvents);

    auto td = QTouchDevice::devices();

    qDebug() << "Before touch device";
    for(auto it = td.begin(); it != td.end(); it++)
    {
      qDebug() << "Touch device name = " << (*it)->name();
    }

    frame->show();

    return app.exec();
}


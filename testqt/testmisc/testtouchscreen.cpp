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

class CButton : public QPushButton
{
public:
  explicit CButton(const QString& _text = QString()) : QPushButton(_text, nullptr)
  {
  }

  virtual bool event(QEvent* _event) override
  {

    switch(_event->type())
    {

    case QEvent::Type::TouchBegin:
    case QEvent::Type::TouchEnd:
    case QEvent::Type::TouchUpdate:
    case QEvent::Type::TouchCancel:
      qDebug() << "Event = " << _event->type();
      break;

    case QEvent::Type::MouseButtonPress:
      qDebug() << "Mouse press";
      break;

    case QEvent::Type::MouseButtonRelease:
      qDebug() << "Mouse release";
      break;

    default:
      break;
    }
    return QPushButton::event(_event);
  }

};

int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    QFrame* frame = new QFrame();
    QPushButton* button1 = new CButton("Button 1");
    QPushButton* button2 = new QPushButton("Button 2");

    button1->setAttribute(Qt::WA_AcceptTouchEvents);
    button1->setAttribute(Qt::WA_TouchPadAcceptSingleTouchEvents);

    QVBoxLayout* layout = new QVBoxLayout();

    layout->addWidget(button1);
    layout->addWidget(button2);

    frame->setLayout(layout);

    button1->setAttribute(Qt::WA_AcceptTouchEvents);
    button1->setAttribute(Qt::WA_TouchPadAcceptSingleTouchEvents);

    auto td = QTouchDevice::devices();

    qDebug() << "Before touch device";
    for(auto it = td.begin(); it != td.end(); it++)
    {
      qDebug() << "Touch device name = " << (*it)->name();
    }

    frame->show();

    return app.exec();
}


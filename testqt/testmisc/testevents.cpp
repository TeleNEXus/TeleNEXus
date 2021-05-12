#include <QApplication>
#include <QtWidgets>
#include <cmath>
#include <ostream>
#include <qboxlayout.h>
#include <qcombobox.h>
#include <QIntValidator>
#include <qglobal.h>
#include <qnamespace.h>
#include <qregexp.h>
#include <qvalidator.h>
#include <QLabel>
#include <QPushButton>
#include <QDebug>
#include <QTextStream>
#include <iostream>

#include <QKeyEvent>

class CEventFilter : public QObject
{
public:
  CEventFilter() : QObject(nullptr){}
  virtual bool eventFilter(QObject* _obj, QEvent* _ev) override
  {
    if(_ev->type() == QEvent::Type::KeyPress) qDebug() << "Event";
    return false;
  }
};

int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    QWidget *mw = new QWidget;

    QVBoxLayout *hlayout = new QVBoxLayout();
    QLineEdit *le_in = new QLineEdit("");

    le_in->installEventFilter(new CEventFilter());


    QPushButton *button_a = new QPushButton("a");
    QPushButton *button_b = new QPushButton("b");
    QPushButton *button_c = new QPushButton("c");

    hlayout->addWidget(le_in);
    hlayout->addWidget(button_a);
    hlayout->addWidget(button_b);
    hlayout->addWidget(button_c);
    
    QObject::connect(button_a, &QPushButton::pressed, 
            [&]()
            {
              QKeyEvent ev = QKeyEvent(QEvent::Type::KeyPress, 
                  Qt::Key::Key_A, Qt::KeyboardModifier::NoModifier);
              QApplication::sendEvent(le_in, &ev);
            });


    QObject::connect(button_b, &QPushButton::pressed, 
            [&]()
            {
              QKeyEvent ev = QKeyEvent(QEvent::Type::KeyPress, 
                  Qt::Key::Key_B, Qt::KeyboardModifier::NoModifier);
              QApplication::sendEvent(le_in, &ev);
            });

    QObject::connect(button_c, &QPushButton::pressed, 
            [&]()
            {
              QKeyEvent ev = QKeyEvent(QEvent::Type::KeyPress, 
                  Qt::Key::Key_C, Qt::KeyboardModifier::NoModifier);
              QApplication::sendEvent(le_in, &ev);
            });

    mw->setLayout( hlayout );        
    mw->show();




    return app.exec();
}

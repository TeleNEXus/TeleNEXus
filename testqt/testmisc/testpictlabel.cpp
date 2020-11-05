
#include <QApplication>
#include <QtWidgets>
#include <qglobal.h>
#include <qnamespace.h>

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    QWidget* widget = new QWidget;
    /* QPushButton* button1 = new QPushButton("Buttonh ___________________1", widget); */
    QPushButton* button1 = new QPushButton("Buttonh ___________________1");
    /* QPushButton* buttonh2 = new QPushButton("Buttonh 2"); */
    /* QPushButton* buttonh3 = new QPushButton("Buttonh 3"); */
    /* QPushButton* buttonh4 = new QPushButton("Buttonh 4"); */
    /* QPushButton* buttonh5 = new QPushButton("Buttonh 5"); */

    button1->setParent(widget);
    button1->move(20, 20);
    
    widget->show();
    return app.exec();
}

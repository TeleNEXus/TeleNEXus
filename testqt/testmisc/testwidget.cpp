
#include <QApplication>
#include <QtWidgets>
#include <qglobal.h>
#include <qnamespace.h>
#include <QScrollArea>
#include <qscrollarea.h>

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    QWidget* widget = new QWidget;
    QScrollArea* sa = new QScrollArea;
    /* QPushButton* button1 = new QPushButton("Buttonh ___________________1", widget); */
    QPushButton* button1 = new QPushButton("Buttonh ___________________1");
    /* QPushButton* buttonh2 = new QPushButton("Buttonh 2"); */
    /* QPushButton* buttonh3 = new QPushButton("Buttonh 3"); */
    /* QPushButton* buttonh4 = new QPushButton("Buttonh 4"); */
    /* QPushButton* buttonh5 = new QPushButton("Buttonh 5"); */

    QVBoxLayout* layout = new QVBoxLayout;

    /* button1->setParent(widget); */
    /* layout->addWidget(button1); */
    /* widget->setLayout(layout); */


    /* button1->move(40, 40); */
    /* button1->setParent(sa); */

    /* sa->setWidget(widget); */
    QLabel* label1 = new QLabel("label1");
    /* layout->addWidget(new QLabel("label1")); */
    /* layout->addWidget(new QLabel("label1")); */
    /* layout->addWidget(new QLabel("label1")); */
    /* layout->addWidget(new QLabel("label1")); */
    /* layout->addWidget(new QLabel("label1")); */
    /* layout->addWidget(label1); */
    /* widget->setLayout(layout); */
    label1->setParent(widget);
    label1->move(50, 50);


    sa->setWidget(widget);

    sa->show();
    widget->resize(150, 150);

    return app.exec();
}

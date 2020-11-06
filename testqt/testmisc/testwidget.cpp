
#include <QApplication>
#include <QtWidgets>
#include <qglobal.h>
#include <qnamespace.h>
#include <QScrollArea>
#include <qscrollarea.h>

#include <QList>

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    QWidget* widget = new QWidget;
    QScrollArea* sa = new QScrollArea;
    /* QPushButton* button1 = new QPushButton("Buttonh ___________________1"); */

    QList<QLabel*> labels;

    QRect wr;
    qDebug() << "widget geometry = " << wr;
        int width = 0;
        int height = 0;
    for(int i = 0; i < 15; i++)
    {

        QLabel* label = new QLabel(QString("Label %1").arg(i));
        label->move(20 * i, 20 * i);
        label->setParent(widget);
        label->show();

        qDebug() << "labe " << i << " geometry = " << label->geometry();
        qDebug() << "labe " << i << " bottomRight = " << label->geometry().bottomRight();

        if(label->geometry().bottomRight().x() > width)
        {
            width = label->geometry().bottomRight().x();
        }

        if(label->geometry().bottomRight().y() > height)
        {
            height = label->geometry().bottomRight().y();
        }
    }
    qDebug() << "width = " << width;
    qDebug() << "height = " << height;

    sa->setWidget(widget);
    qDebug() << "widget geometry = " << wr;

    widget->resize(width, height);
    /* widget->setFixedSize(width, height); */
    /* sa->setFixedSize(width, height); */
    sa->resize(width +10, height +10);
    sa->setAlignment(Qt::AlignmentFlag::AlignCenter);

    
    sa->show();


    return app.exec();
}


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
    QFrame* widget = new QFrame;
    QScrollArea* sa = new QScrollArea;
    QLabel* label = new QLabel("label test");
    /* QPushButton* button1 = new QPushButton("Buttonh ___________________1"); */

    QList<QLabel*> labels;

    QRect wr;
    qDebug() << "widget geometry = " << wr;
        int width = 0;
        int height = 0;
    for(int i = 0; i < 5; i++)
    {

        /* QLabel* label = new QLabel(QString("Label %1").arg(i)); */
        QPushButton* label = new QPushButton(QString("Label %1").arg(i));
        label->move(10 + 20 * i, 10 + 25 * i);
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
        /* QPalette pal(label->palette()); */
        /* pal.setColor(QPalette::ColorRole::Background, Qt::GlobalColor::magenta); */
        /* label->setAutoFillBackground(true); */
        /* label->setPalette(pal); */
    }

    qDebug() << "width = " << width;
    qDebug() << "height = " << height;

        /* QPalette pal(widget->palette()); */
        /* pal.setColor(QPalette::ColorRole::Background, Qt::GlobalColor::blue); */
        /* pal.setColor(QPalette::ColorRole::ButtonText, Qt::GlobalColor::cyan); */
        /* pal.setColor(QPalette::ColorRole::ButtonText, Qt::GlobalColor::magenta); */
        /* /1* widget->setAutoFillBackground(true); *1/ */
        /* widget->setPalette(pal); */

    sa->setWidget(widget);
    qDebug() << "widget geometry = " << wr;

    
    /* widget->setFrameStyle(QFrame::Shadow::Plain | QFrame::Shape::HLine); */
    widget->setFrameStyle(QFrame::Shadow::Raised | QFrame::Shape::WinPanel);
    widget->setLineWidth(5);
    widget->resize(width + 100, height + 100);
    /* widget->setFixedSize(width, height); */
    /* sa->setFixedSize(width, height); */
    sa->resize(width +150, height +150);
    sa->setAlignment(Qt::AlignmentFlag::AlignCenter);
    /* sa->setFrameStyle(QFrame::NoFrame); */

    /* sa->setWindowFlag(Qt::WindowType::CustomizeWindowHint | Qt::WindowType::FramelessWindowHint); */
    
    /* sa->setWindowFlag(Qt::WindowType::FramelessWindowHint); */
        /* sa->setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint); */
        /* sa->setWindowFlags(Qt::Window  | Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint); */
        /* sa->setWindowFlags(Qt::Window  | Qt::FramelessWindowHint); */
    sa->show();

    /* sa->setWindowFlag(Qt::WindowType::CustomizeWindowHint); */
    /* sa->setWindowFlag(Qt::WindowType::FramelessWindowHint); */

    return app.exec();
}

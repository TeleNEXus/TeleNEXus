
#include <QApplication>
#include <QtWidgets>
#include <qglobal.h>
#include <qnamespace.h>
#include <QScrollArea>
#include <qscrollarea.h>
#include <QComboBox>

#include <QList>

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    QFrame* widget = new QFrame;
    QScrollArea* sa = new QScrollArea;
    /* QLabel* label = new QLabel("label test"); */
    /* QPushButton* button1 = new QPushButton("Buttonh ___________________1"); */

    QList<QLabel*> labels;

    QRect wr;
    qDebug() << "widget geometry = " << wr;
        int width = 0;
        int height = 0;
    for(int i = 0; i < 5; i++)
    {

        /* QLabel* label = new QLabel(QString("Label00 %1").arg(i)); */
        /* QPushButton* label = new QPushButton(QString("Label %1").arg(i)); */
        /* QLineEdit* label = new QLineEdit(QString("Label %1").arg(i)); */
        QComboBox* label = new QComboBox;
        label->addItem("text item 0");
        label->addItem("text item 1");
        label->addItem("text item 2");
        label->addItem("text item 3");


        label->move(10 + 20 * i, 10 + 45 * i);
        label->setParent(widget);
        label->show();
        QFont font = label->font();
        font.setFamily("DejaVu Sans Mono");
        /* font.setPixelSize(50); */
        font.setPointSize(18);
        label->setFont(font);
        label->adjustSize();
        

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
        label->setAutoFillBackground(true);
        /* label->setPalette(pal); */
        QPalette pal = label->palette();
        /* pal.setColor(QPalette::ColorRole::Background, Qt::GlobalColor::red); */
        
        //Label
        /* pal.setColor(QPalette::ColorRole::Background, QColor(QRgb(0x000000ff))); */
        /* pal.setColor(QPalette::ColorRole::Foreground, QColor(QRgb(0x00ff0000))); */

        //LineEdit
        /* pal.setColor(QPalette::ColorRole::Base, QColor(QRgb(0x000000ff))); */
        /* pal.setColor(QPalette::ColorRole::Text, QColor(QRgb(0x00ff0000))); */

        //Button ComboBox
        pal.setColor(QPalette::ColorRole::Button, QColor(QRgb(0x000000ff)));
        pal.setColor(QPalette::ColorRole::ButtonText, QColor(QRgb(0x00ff0000)));

        label->setPalette(pal);
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
    QPalette pal = widget->palette();

    /* pal.setColor(QPalette::ColorRole::Background, Qt::GlobalColor::red); */
    pal.setColor(QPalette::ColorRole::Background, QColor(QRgb(0x00aaaaff)));

    widget->setAutoFillBackground(true);
    widget->setPalette(pal);
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

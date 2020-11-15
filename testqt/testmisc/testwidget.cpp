
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

        QLabel* label = new QLabel(QString("Label00 %1").arg(i));
        /* QPushButton* label = new QPushButton(QString("Label %1").arg(i)); */
        /* QLineEdit* label = new QLineEdit(QString("Label %1").arg(i)); */
        /* QComboBox* label = new QComboBox; */
        /* label->addItem("text item 0"); */
        /* label->addItem("text item 1"); */
        /* label->addItem("text item 2"); */
        /* label->addItem("text item 3"); */


        label->move(10 + 20 * i, 10 + 45 * i);
        label->setParent(widget);
        label->show();
        QFont font = label->font();
        font.setFamily("DejaVu Sans Mono");
        /* font.setPixelSize(50); */
        font.setPointSize(18);
        label->setFont(font);
        label->adjustSize();
        label->resize(label->sizeHint().width() + 50, label->sizeHint().height() + 15);
        label->setAlignment(Qt::AlignmentFlag::AlignHCenter);
        

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
        pal.setColor(QPalette::ColorRole::Background, QColor(QRgb(0x000000ff)));
        pal.setColor(QPalette::ColorRole::Foreground, QColor(QRgb(0x00ff0000)));

        //LineEdit
        /* pal.setColor(QPalette::ColorRole::Base, QColor(QRgb(0x000000ff))); */
        /* pal.setColor(QPalette::ColorRole::Text, QColor(QRgb(0x00ff0000))); */

        //Button ComboBox
        /* pal.setColor(QPalette::ColorRole::Button, QColor(QRgb(0x000000ff))); */
        /* pal.setColor(QPalette::ColorRole::ButtonText, QColor(QRgb(0x00ff0000))); */

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

    qDebug() << "widget geometry = " << wr;
    
    /* widget->setFrameStyle(QFrame::Shadow::Plain | QFrame::Shape::HLine); */
    widget->setFrameStyle(QFrame::Shadow::Raised | QFrame::Shape::WinPanel);
    widget->setLineWidth(5);
    /* widget->resize(width + 300, height + 300); */
    widget->setFixedSize(500, 500);
    widget->resize(500, 500);
    QPixmap back_pict;
    qDebug() << "back pict is null = " << back_pict.isNull();
    back_pict.load("/home/serg/pprj/tnex/xmltestprj/linux/prj1/picture/fon1.png");
    qDebug() << "back pict is null = " << back_pict.isNull();
    /* back_pict = back_pict.scaled(500, 500); */
    back_pict = back_pict.scaled(widget->size().width(), widget->size().height());
    QPalette pal = widget->palette();

    /* pal.setColor(QPalette::ColorRole::Background, Qt::GlobalColor::red); */
    /* pal.setColor(QPalette::ColorRole::Background, QColor(QRgb(0x00aaaaff))); */
    pal.setBrush(QPalette::ColorRole::Background, QBrush(back_pict));

    widget->setAutoFillBackground(true);
    widget->setPalette(pal);
    /* widget->show(); */
    pal = sa->palette();
    pal.setColor(QPalette::ColorRole::Background, Qt::GlobalColor::red);
    sa->setPalette(pal);

    sa->setWidget(widget);
    sa->resize(width +150, height +150);
    sa->setAlignment(Qt::AlignmentFlag::AlignCenter);

    sa->show();


    return app.exec();
}

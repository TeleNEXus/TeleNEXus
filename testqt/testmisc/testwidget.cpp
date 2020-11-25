
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
    for(int i = 0; i < 2; i++)
    {

        QLabel* label = new QLabel(QString("Label00 %1").arg(i));
        /* QPushButton* label = new QPushButton(QString("Label %1").arg(i)); */
        /* QLineEdit* label = new QLineEdit(QString("Label %1").arg(i)); */
        /* QComboBox* label = new QComboBox; */
        /* label->addItem("text item 0"); */
        /* label->addItem("text item 1"); */
        /* label->addItem("text item 2"); */
        /* label->addItem("text item 3"); */


        label->move(30 + 20 * i, 30 + 45 * i);
        label->setParent(widget);
        label->show();
        QFont font = label->font();
        font.setFamily("DejaVu Sans Mono");
        /* font.setPixelSize(50); */
        font.setPointSize(18);
        label->setFont(font);
        label->adjustSize();
        label->resize(label->sizeHint().width() + 50, label->sizeHint().height() + 15);
        /* label->setAlignment(Qt::AlignmentFlag::AlignHCenter); */
        

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

    QString picture_1 = "/home/serg/pprj/tnex/xmltestprj/linux/prj1/picture/picture_1.png";
    QString picture_2 = "/home/serg/pprj/tnex/xmltestprj/linux/prj1/picture/picture_2.png";
    QStackedWidget *sw = new QStackedWidget(widget);
    sw->move(150, 150);
    QLabel* label_1 = new QLabel("label_1");
    QLabel* label_2 = new QLabel("label_2");
    label_1->setStyleSheet(".QLabel {color: red; font : 10pt;background : yellow; qproperty-alignment: AlignRight;}");
    label_2->setStyleSheet(".QLabel {color: yellow;font : 20pt;}");
    QPushButton* button_0 = new QPushButton("change button 0", widget);
    button_0->move(250,0);
    /* button_0->setStyleSheet("QWidget {qproperty-pos: QPoint(100,100) }"); */

    QObject::connect(button_0, &QPushButton::pressed, 
            [&]{
                sw->setCurrentWidget(sw->widget(0));
            });

    QPushButton* button_1 = new QPushButton("change button 1", widget);
    button_1->move(250,30);
    QObject::connect(button_1, &QPushButton::pressed, 
            [&]{

                sw->setCurrentWidget(sw->widget(1));
                button_1->setIcon(QIcon(picture_2));
                qDebug() << "Button 1 pressed";
            });

    QObject::connect(button_1, &QPushButton::released, 
            [&]{

                button_1->setIcon(QIcon(picture_1));
                qDebug() << "Button 1 released";
            });
    sw->addWidget(label_1);
    sw->addWidget(label_2);
    button_0->setIcon(QIcon(picture_1));

    /* button_0->setStyleSheet("background: red;min-width: 100px;"); */
    /* QString style_sheet = QString("background-image:url(\"%1\"); ").arg(picture_1); */
    QString style_sheet;
    /* style_sheet += "min-width: 40px   ;"; */
    /* style_sheet += "min-height: 40px  ;"; */
    style_sheet += "max-width: 40px   ;";
    style_sheet += "max-height: 40px  ;";
    /* style_sheet += "border-width: 0px  ;"; */
    button_1->setIcon(QIcon(picture_1));
    button_1->setIconSize(QSize(50,50));

    /* button_1->setText("button 1"); */
    button_1->setText("");
    button_1->setStyleSheet(style_sheet);
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

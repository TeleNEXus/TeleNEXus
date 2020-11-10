
#include <QApplication>
#include <QtWidgets>
#include <qglobal.h>
#include <qnamespace.h>
#include <qpicture.h>

volatile float c;

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    QWidget* widget = new QWidget;

    QPushButton* buttonh1 = new QPushButton("Buttonh 1");
    QPushButton* buttonh2 = new QPushButton("Buttonh 2");
    QPushButton* buttonh3 = new QPushButton("Buttonh 3");
    QPushButton* buttonh4 = new QPushButton("Buttonh 4");
    QPushButton* buttonh5 = new QPushButton("Buttonh 5");

    QPushButton* buttonv1 = new QPushButton("Buttonv 1");
    QPushButton* buttonv2 = new QPushButton("Buttonv 2");
    QPushButton* buttonv3 = new QPushButton("Buttonv 3");
    QPushButton* buttonv4 = new QPushButton("Buttonv 4");
    QPushButton* buttonv5 = new QPushButton("Buttonv 5");

    QLabel* label1 = new QLabel("Label 1 1234 124  1234 1234 1234 2134 2134 "
            "2134 24 234 234 234 234 234 234 234");
    QLabel* label2 = new QLabel("Label 2");
    QLabel* label3 = new QLabel("Label 3");
    QLabel* label4 = new QLabel("Label 4");
    QLabel* label5 = new QLabel("Label 5");

    QHBoxLayout* hlayout1 = new QHBoxLayout;
    QVBoxLayout* vlayout1 = new QVBoxLayout;

    hlayout1->addSpacing(0);
    hlayout1->setSpacing(-220);
    hlayout1->addWidget(buttonh1);
    hlayout1->addStretch();
    hlayout1->addWidget(buttonh2);
    hlayout1->addWidget(buttonh3);
    hlayout1->addWidget(buttonh4);
    hlayout1->addWidget(buttonh5);

    vlayout1->addLayout(hlayout1);

    vlayout1->addWidget(buttonv1);
    vlayout1->addStretch(0);
    vlayout1->addWidget(buttonv2);
    vlayout1->addWidget(buttonv3);
    vlayout1->addWidget(buttonv4);
    vlayout1->addWidget(buttonv5);
     

    /* QPixmap pix; */
    /* if(!pix.load("/home/serg/pprj/tnex/testqt/testmisc/pict/clock.gif")) */
    /*     qDebug() << "Can't load Pix map"; */
    /* qDebug() << "pict alpha = " << pix.hasAlpha(); */

    /* pix.setMask(pix.createMaskFromColor(Qt::GlobalColor::white, Qt::MaskMode::MaskInColor)); */ 

    /* label2->setPixmap(pix); */

    QMovie *mv = new QMovie(label2);
    QMovie *mv1 = new QMovie(label3);
    QMovie *mv2 = new QMovie(label4);
    /* mv.setFileName("/home/serg/pprj/tnex/testqt/testmisc/pict/clock.gif"); */
    /* mv.setFileName("/home/serg/pprj/tnex/testqt/testmisc/pict/box1.bmp"); */
    /* mv.setFileName("/home/serg/pprj/tnex/testqt/testmisc/pict/clocl_1.gif"); */
    mv->setFileName("/home/serg/pprj/tnex/testqt/testmisc/pict/clocl_1.gif");
    mv->setBackgroundColor(QColor(Qt::GlobalColor::transparent));
    qDebug() << "backgroundColor = " << mv->backgroundColor();
    mv1->setFileName("/home/serg/pprj/tnex/testqt/testmisc/pict/clocl_1.gif");
    mv1->setBackgroundColor(QColor(Qt::GlobalColor::transparent));
    qDebug() << "backgroundColor = " << mv1->backgroundColor();
    mv2->setFileName("/home/serg/pprj/tnex/testqt/testmisc/pict/clocl_1.gif");
    mv2->setBackgroundColor(QColor(Qt::GlobalColor::transparent));
    qDebug() << "backgroundColor = " << mv2->backgroundColor();

    /* label2->setMovie(mv); */
    label3->setMovie(mv1);
    label4->setMovie(mv2);
    /* label2->setSizePolicy(QSizePolicy::Policy::MinimumExpanding, QSizePolicy::Policy::MinimumExpanding); */

    mv1->start();
    mv2->start();

    label2->setAlignment(Qt::AlignmentFlag::AlignCenter);

    label1->setWordWrap(true);
    label1->setAlignment(Qt::AlignmentFlag::AlignCenter);
    vlayout1->addWidget(label1);
    vlayout1->addWidget(label2);
    vlayout1->addWidget(label3);
    vlayout1->addWidget(label4);
    vlayout1->addWidget(label5);

    widget->setWindowTitle("Test layouts");
    widget->setLayout(vlayout1);

    QObject::connect(buttonh1, &QPushButton::pressed, 
            [=]()
            {
                label2->clear();
                label2->setMovie(mv);
                mv->start();

                /* label2->resize(100, 100); */
                /* label2->adjustSize(); */
                /* label2->show(); */
                /* label2->resize(label2->sizeHint()); */
                /* label2->updateGeometry(); */
                qDebug() << "label 2 size hint " << label2->sizeHint();
                qDebug() << "label 2 size  " << label2->size();
                qDebug() << "label 2 base size  " << label2->baseSize();
                /* label2->resize(50, 50); */
                /* label2->update(); */
                /* label2->repaint(); */

                /* static_cast<QLayout*>(label2->parent())->update(); */
            });

    QObject::connect(buttonh2, &QPushButton::pressed, 
            [=]()
            {
                label2->clear();
                label2->setText("new text");
                mv->stop();
            });


    widget->show();
    return app.exec();
}

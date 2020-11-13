
#include <QApplication>
#include <QtWidgets>
#include <qglobal.h>
#include <qnamespace.h>
#include <qpicture.h>

volatile float c;
class CWidget : public QWidget
{
private:
    QSize mSizeHint;
public:
    explicit CWidget(QWidget* _parent=nullptr) : QWidget(_parent){}
    virtual QSize sizeHint() const override
    {
        return QSize(mSizeHint.width()+5, mSizeHint.height()+5);
    }

    void addWidget(QWidget* _widget)
    {
        int width   = mSizeHint.width();
        int height  = mSizeHint.height();

        _widget->setParent(this);
        _widget->show();
        
        qDebug() << "mSizeHint 0 = " << mSizeHint;


        if( _widget->geometry().bottomRight().x() > width)
        {
            width = _widget->geometry().bottomRight().x();
        }

        if( _widget->geometry().bottomRight().y() > height)
        {
            height = _widget->geometry().bottomRight().y();
        }

        mSizeHint = QSize(width, height);
        qDebug() << "mSizeHint 1 = " << mSizeHint;
        /* this->setGeometry(0,0,mSizeHint.width(),mSizeHint.height()); */
    }

};

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    CWidget* widget = new CWidget;
    QWidget* widget1 = new QWidget;
    QVBoxLayout* layout = new QVBoxLayout;

    QPushButton* button0 = new QPushButton("Button 0");
    QPushButton* button1 = new QPushButton("Button 1");

    QPushButton* button2 = new QPushButton("Button 2");
    QPushButton* button3 = new QPushButton("Button 3");

    QLabel* label0 = new QLabel("Label 0");
    QLabel* label1 = new QLabel("Label 1");
    QLabel* label2 = new QLabel("Label 2");
                                        
    QLabel* label3 = new QLabel("Label 3");
    QLabel* label4 = new QLabel("Label 4");
    QLabel* label5 = new QLabel("Label 5");

    QMovie *mv0 = new QMovie(label0);
    QMovie *mv1 = new QMovie(label1);
    QMovie *mv2 = new QMovie(label2);

    QMovie *mv3 = new QMovie(label3);
    QMovie *mv4 = new QMovie(label4);
    QMovie *mv5 = new QMovie(label5);


    /* button0->setParent(widget); */
    /* button1->setParent(widget); */

    /* label0->setParent(widget); */
    /* label1->setParent(widget); */
    /* label2->setParent(widget); */

    button0->move(100, 120);
    button1->move(100, 150);

    label0->move(0,   0);
    label1->move(100, 0);
    label2->move(200, 0);

    mv0->setFileName("/home/serg/pprj/tnex/testqt/testmisc/pict/clocl_1.gif");
    /* mv0->setBackgroundColor(QColor(Qt::GlobalColor::transparent)); */
    mv1->setFileName("/home/serg/pprj/tnex/testqt/testmisc/pict/clocl_1.gif");
    /* mv1->setBackgroundColor(QColor(Qt::GlobalColor::transparent)); */
    mv2->setFileName("/home/serg/pprj/tnex/testqt/testmisc/pict/clocl_1.gif");
    /* mv2->setBackgroundColor(QColor(Qt::GlobalColor::transparent)); */
    mv3->setFileName("/home/serg/pprj/tnex/testqt/testmisc/pict/clocl_1.gif");
    mv4->setFileName("/home/serg/pprj/tnex/testqt/testmisc/pict/clocl_1.gif");
    mv5->setFileName("/home/serg/pprj/tnex/testqt/testmisc/pict/clocl_1.gif");

    /* label0->setMovie(mv0); */
    label1->setMovie(mv1);
    label2->setMovie(mv2);

    /* label3->setMovie(mv3); */
    label4->setMovie(mv4);
    label5->setMovie(mv5);

    /* mv0->start(); */
    mv1->start();
    mv2->start();
    /* mv3->start(); */
    mv4->start();
    mv5->start();

    /* label2->setAlignment(Qt::AlignmentFlag::AlignCenter); */

    widget->setWindowTitle("Test layouts");

                /* label0->clear(); */
                label0->clear();
                label0->setMovie(mv0);
                widget->setFixedSize(100,100);
    QObject::connect(button0, &QPushButton::pressed, 
            [=]()
            {
            static int frame = 0;
                
                qDebug() << "Move size = "    << mv0->currentPixmap().size();
                /* mv0->start(); */
                /* qDebug() << "Move loop count = "    << mv0->loopCount(); */
                qDebug() << "Move next frame = "    << mv0->jumpToFrame(frame);
                qDebug() << "Move frame = "    << frame;

                frame++;

                
                /* mv0->start(); */
                /* label0->clear(); */
                /* label0->setMovie(mv0); */


                /* /1* label0->resize(100, 100); *1/ */
                /* label0->adjustSize(); */
                /* /1* widget->adjustSize(); *1/ */
                /* /1* widget->setMinimumSize(200,200); *1/ */
            
                /* /1* widget->setFixedSize(200,200); *1/ */
                /* /1* label0->show(); *1/ */
                /* /1* label0->resize(label0->sizeHint()); *1/ */
                /* /1* label0->updateGeometry(); *1/ */
                /* qDebug() << "label 0 size hint "    << label0->sizeHint(); */
                /* qDebug() << "label 0 size  "        << label0->size(); */
                /* qDebug() << "label 0 base size  "   << label0->baseSize(); */
                /* /1* label0->resize(50, 50); *1/ */
                /* /1* label0->update(); *1/ */
                /* /1* label0->repaint(); *1/ */

                /* /1* static_cast<QLayout*>(label0->parent())->update(); *1/ */
                /* qDebug() << "widget size hint "    << widget->sizeHint(); */
                /* qDebug() << "widget size  "        << widget->size(); */
                /* qDebug() << "widget base size  "   << widget->baseSize(); */
            });

    QObject::connect(button1, &QPushButton::pressed, 
            [=]()
            {
                /* label0->clear(); */
                /* label0->setText("new text"); */
                /* mv0->stop(); */
                /* widget->adjustSize(); */
                /* qDebug() << "widget size hint "    << widget->sizeHint(); */
                /* qDebug() << "widget size  "        << widget->size(); */
                /* qDebug() << "widget base size  "   << widget->baseSize(); */
            });

    /* QObject::connect(button2, &QPushButton::pressed, */ 
    /*         [=]() */
    /*         { */
    /*             label3->clear(); */
    /*             label3->setMovie(mv3); */
    /*             mv3->start(); */

    /*             /1* label0->resize(100, 100); *1/ */
    /*             label3->adjustSize(); */
    /*             /1* label0->show(); *1/ */
    /*             /1* label0->resize(label0->sizeHint()); *1/ */
    /*             /1* label3->updateGeometry(); *1/ */
    /*             qDebug() << "label 3 size hint "    << label3->sizeHint(); */
    /*             qDebug() << "label 3 size  "        << label3->size(); */
    /*             qDebug() << "label 3 base size  "   << label3->baseSize(); */
    /*             qDebug() << "widget size hint "    << widget->sizeHint(); */
    /*             qDebug() << "widget size  "        << widget->size(); */
    /*             qDebug() << "widget base size  "   << widget->baseSize(); */
    /*             /1* label0->resize(50, 50); *1/ */
    /*             /1* label0->update(); *1/ */
    /*             /1* label0->repaint(); *1/ */

    /*             /1* static_cast<QLayout*>(label0->parent())->update(); *1/ */
    /*         }); */

    /* QObject::connect(button3, &QPushButton::pressed, */ 
    /*         [=]() */
    /*         { */
    /*             label3->clear(); */
    /*             label3->setText("new text"); */
    /*             mv3->stop(); */
    /*             qDebug() << "label 3 size hint "    << label3->sizeHint(); */
    /*             qDebug() << "label 3 size  "        << label3->size(); */
    /*             qDebug() << "label 3 base size  "   << label3->baseSize(); */
    /*             qDebug() << "widget size hint "    << widget->sizeHint(); */
    /*             qDebug() << "widget size  "        << widget->size(); */
    /*             qDebug() << "widget base size  "   << widget->baseSize(); */
    /*         }); */
    /* widget->setMinimumSize(300, 300); */

    /* widget->resize(300, 300); */

    button0->show();
    button1->show();

    /* label0->show(); */
    /* label1->show(); */
    /* label2->show(); */

    /* widget->addWidget(button0); */
    /* widget->addWidget(button1); */

    widget->addWidget(label0);
    /* widget->addWidget(label1); */
    /* widget->addWidget(label2); */


    widget->adjustSize();
    /* layout->addWidget(widget); */

    layout->addWidget(button2);
    layout->addWidget(button3);
    layout->addWidget(label3);
    layout->addWidget(label4);
    layout->addWidget(label5);

    widget1->setLayout(layout);
    /* widget1->show(); */
    /* widget->show(); */
    /* widget->resize(300, 300); */
    /* widget->adjustSize(); */
    /* widget->setGeometry(0,0,100,100); */
    QScrollArea* scrollarea = new QScrollArea;
    scrollarea->setWidget(widget);
    scrollarea->setAlignment(Qt::AlignmentFlag::AlignCenter);
    scrollarea->show();


    return app.exec();
}

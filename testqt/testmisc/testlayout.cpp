
#include <QApplication>
#include <QtWidgets>
#include <qglobal.h>
#include <qnamespace.h>

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    QWidget* widget = new QWidget;
    QPushButton* buttonh1 = new QPushButton("Buttonh ___________________1");
    QPushButton* buttonh2 = new QPushButton("Buttonh 2");
    QPushButton* buttonh3 = new QPushButton("Buttonh 3");
    QPushButton* buttonh4 = new QPushButton("Buttonh 4");
    QPushButton* buttonh5 = new QPushButton("Buttonh 5");

    QPushButton* buttonv1 = new QPushButton("Buttonv 1");
    QPushButton* buttonv2 = new QPushButton("Buttonv 2");
    QPushButton* buttonv3 = new QPushButton("Buttonv 3");
    QPushButton* buttonv4 = new QPushButton("Buttonv 4");
    QPushButton* buttonv5 = new QPushButton("Buttonv 5_______ _____ _____");

    QLabel* label1 = new QLabel("Label 1 1234 124  1234 1234 1234 2134 2134 2134 24 234 234 234 234 234 234 234");
    QLabel* label2 = new QLabel("Label 2");
    QLabel* label3 = new QLabel("Label 3");
    QLabel* label4 = new QLabel("Label 4");
    QLabel* label5 = new QLabel("Label 5 asdf asdf asdf asdf asfd asfd sadf asdf asdf asdf asdf saf");

    QTableWidget* table = new QTableWidget;
    table->setRowCount(3);
    table->setColumnCount(5);
    label5->setWordWrap(true);
    /* table->verticalHeader()->setStretchLastSection(true); */
    /* table->horizontalHeader()->setSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding); */
    /* table->verticalHeader()->setSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding); */
    table->setCellWidget(0, 0, label4);   
    table->setCellWidget(1, 0, label5);   
    table->setCellWidget(1, 2, buttonv5);   
    table->horizontalHeader()->setSectionResizeMode(
            QHeaderView::ResizeMode::Interactive);
    table->resizeColumnsToContents(); 
    /*         table->updateGeometry(); */
    /* table->horizontalHeader()->repaint(); */
    /* table->horizontalHeader()->setSectionResizeMode( */
    /*         QHeaderView::ResizeMode::Interactive); */
    /* table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::Interactive); */
    /* table->resizeColumnsToContents(); */ 
    QHBoxLayout* hlayout1 = new QHBoxLayout;
    QVBoxLayout* vlayout1 = new QVBoxLayout;

    hlayout1->addSpacing(100);
    hlayout1->addWidget(buttonh1);
    hlayout1->addStretch(0);
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
    /* vlayout1->addWidget(buttonv5); */
    vlayout1->addWidget(table);

    QPixmap pix;
    if(!pix.load("/home/serg/pprj/tnex/xmltestprj/linux/prj1/pict/pict1.jpg"))
        qDebug() << "Can't load Pix map";
    label2->setPixmap(pix);
    label2->setAlignment(Qt::AlignmentFlag::AlignCenter);
    label1->setWordWrap(true);
    label1->setAlignment(Qt::AlignmentFlag::AlignCenter);
    vlayout1->addWidget(label1);
    vlayout1->addWidget(label2);
    vlayout1->addWidget(label3);
    /* vlayout1->addWidget(label4); */
    /* vlayout1->addWidget(label5); */

    widget->setWindowTitle("Test layouts");
    widget->setLayout(vlayout1);
    widget->show();
    return app.exec();
}


#include <QApplication>
#include <QtWidgets>
#include <qglobal.h>
#include <qnamespace.h>

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

    QLabel* label1_0 = new QLabel("Label 1_0");
    QLabel* label2_0 = new QLabel("Label 2_0");
    QLabel* label3_0 = new QLabel("Label 3_0");
    QLabel* label4_0 = new QLabel("Label 4_0");
    QLabel* label5_0 = new QLabel("Label 5_0");

    QLabel* label1_1 = new QLabel("Label 1_1");
    QLabel* label2_1 = new QLabel("Label 2_1");
    QLabel* label3_1 = new QLabel("Label 3_1");
    QLabel* label4_1 = new QLabel("Label 4_1");
    QLabel* label5_1 = new QLabel("Label 5_1");

    QLabel* label1_2 = new QLabel("Label 1_2");
    QLabel* label2_2 = new QLabel("Label 2_2");
    QLabel* label3_2 = new QLabel("Label 3_2");
    QLabel* label4_2 = new QLabel("Label 4_2");
    QLabel* label5_2 = new QLabel("Label 5_2");

    QGridLayout* glayout = new QGridLayout();

    /* glayout->setColumnMinimumWidth(1, 300); */
    /* glayout->addItem(new QSpacerItem(0,100), 0, 0); */
    /* glayout->setHorizontalSpacing(200); */
    glayout->addWidget(label1_0, 0, 0);
    glayout->addWidget(label2_0, 1, 0);
    glayout->addWidget(label3_0, 2, 0);
    glayout->addWidget(label4_0, 3, 0);
    glayout->addWidget(label5_0, 4, 0);

    /* glayout->addItem(new QSpacerItem(0,100), 0, 0, 1, 1, Qt::AlignmentFlag::AlignLeft); */
    glayout->addWidget(label1_1, 0, 1);
    glayout->addWidget(label2_1, 1, 1);
    glayout->addWidget(label3_1, 2, 1);
    glayout->addWidget(label4_1, 3, 1);
    glayout->addWidget(label5_1, 4, 1);

    glayout->addWidget(label1_2, 0, 2);
    glayout->addWidget(label2_2, 1, 2);
    glayout->addWidget(label3_2, 2, 2);
    glayout->addWidget(label4_2, 3, 2);
    glayout->addWidget(label5_2, 4, 2);

    glayout->setColumnStretch(0, 0);
    /* glayout->setRowStretch(0, 1); */

    /* glayout->setColumnStretch(0, 1); */
    /* glayout->setColumnStretch(1, 1); */

    widget->setWindowTitle("Test layouts");
    widget->setLayout(glayout);
    widget->show();

    return app.exec();
}

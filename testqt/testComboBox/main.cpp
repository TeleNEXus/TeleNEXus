#include <QApplication>
#include <QtWidgets>
#include <qboxlayout.h>
#include <qcombobox.h>
#include <QIntValidator>
#include <qglobal.h>
#include <qvalidator.h>
int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    QWidget *mw = new QWidget;

    QComboBox *cb1 = new QComboBox();

    cb1->insertItem(0, "item_10", QVariant(10));
    cb1->insertItem(1, "item_20", QVariant(20));
    cb1->insertItem(2, "item_30", QVariant(30));
    cb1->insertItem(3, "item_40", QVariant(40));


    QComboBox *cb2 = new QComboBox();
    cb2->insertItem(0, "item_10", QVariant(10));
    cb2->insertItem(1, "item_20", QVariant(20));
    cb2->insertSeparator(1);
    cb2->insertItem(2, "item_30", QVariant(30));
    cb2->insertItem(3, "item_40", QVariant(40));

    QComboBox *cb3 = new QComboBox();

    QPushButton *button1 = new QPushButton("IndexUp");
    QPushButton *button2 = new QPushButton("IndexDown");
    QLineEdit *lineEdit1 = new QLineEdit("0");
    lineEdit1->setValidator(new QIntValidator(-50,50,lineEdit1));
    QHBoxLayout *hlayout = new QHBoxLayout();

    hlayout->addWidget(cb1);
    hlayout->addWidget(cb2);
    hlayout->addWidget(cb3);
    hlayout->addWidget(button1);
    hlayout->addWidget(button2);
    hlayout->addWidget(lineEdit1);

    QObject::connect(button1, &QPushButton::pressed, 
            [&]()
            {
                qDebug() << "Push Button Up";
                cb1->setCurrentIndex(2);

            });

    QObject::connect(button2, &QPushButton::pressed, 
            [&]()
            {
                qDebug() << "Push Button Down";
                cb1->setCurrentIndex(-1);
            });

    QObject::connect(cb1, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), 
            [&](int index)
            {
                Q_UNUSED(index);
                lineEdit1->setText(QString::number(cb1->currentData().toInt()));
            });

    QObject::connect(lineEdit1, &QLineEdit::editingFinished, 
            [&]()
            {
                cb1->setCurrentIndex(cb1->findData(QVariant(lineEdit1->text().toInt())));
            });

/*     lineEdit1->setEnabled(false); */
    /* cb1->setEnabled(false); */
    mw->setLayout( hlayout );    

    mw->show();
    return app.exec();
}

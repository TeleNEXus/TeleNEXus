
#include <QApplication>
#include <QtWidgets>
#include <qboxlayout.h>
#include <qfontmetrics.h>
#include <qglobal.h>
#include <qnamespace.h>
#include <QListWidget>
#include <qsizepolicy.h>
#include <qstackedlayout.h>
#include <QSplitter>
#include <QFontMetrics>

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    QListWidget *listWidget = new QListWidget;
    QHBoxLayout *boxLayout = new QHBoxLayout;
    QStackedLayout *stackLayout = new QStackedLayout;
    QWidget *widget = new QWidget;
    QSplitter *splitter = new QSplitter(Qt::Orientation::Horizontal);

    /* listWidget->setMaximumWidth(75 + 10); */
    /* listWidget->setFixedWidth(100); */
    /* listWidget->setMinimumWidth(100); */
    listWidget->setMaximumWidth(100);
    /* listWidget->setSizePolicy(QSizePolicy::Policy::Minimum, */ 
    /*         QSizePolicy::Policy::Minimum); */

    QLabel *label0 = new QLabel("Widget set 0__________________________________________");
    QLabel *label1 = new QLabel("Widget set 1");
    QLabel *label2 = new QLabel("Widget set 2");
    QLabel *label3 = new QLabel("Widget set 3");
    QLabel *label4 = new QLabel("Widget set 4");
    QLabel *label5 = new QLabel("Widget set 5");


    stackLayout->addWidget(label0);
    stackLayout->addWidget(label1);
    stackLayout->addWidget(label2);
    stackLayout->addWidget(label3);
    stackLayout->addWidget(label4);
    stackLayout->addWidget(label5);


    listWidget->addItem("Widget set 0");
    listWidget->addItem("Widget set 1");
    listWidget->addItem("Widget set 2");
    listWidget->addItem("Widget set 3");
    listWidget->addItem("Widget set 4");
    listWidget->addItem("Widget set 5");

    listWidget->setBaseSize(QSize(100, 100));
    /* QListWidgetItem *item0 = new QListWidgetItem("Widget set 0"); */
    /* QListWidgetItem *item1 = new QListWidgetItem("Widget set 1"); */
    /* QListWidgetItem *item2 = new QListWidgetItem("Widget set 2"); */
    /* QListWidgetItem *item3 = new QListWidgetItem("Widget set 3"); */
    /* QListWidgetItem *item4 = new QListWidgetItem("Widget set 4"); */
    /* QListWidgetItem *item5 = new QListWidgetItem("Widget set 5"); */
    /* QFontMetrics fm(listWidget->font()); */
    /* qDebug() << "font metric width = " << fm.width("Widget set 000000"); */
    /* QSize s = fm.boundingRect("Widget set 0").size(); */
    /* qDebug() << "size hint = " << s; */
    /* item0->setSizeHint(s); */



    /* listWidget->addItem(item0); */
    /* listWidget->addItem(item1); */
    /* listWidget->addItem(item2); */
    /* listWidget->addItem(item3); */
    /* listWidget->addItem(item4); */
    /* listWidget->addItem(item5); */
    /* boxLayout->addWidget(listWidget); */
    /* boxLayout->addStretch(); */
    /* boxLayout->addLayout(stackLayout); */

    QObject::connect(listWidget, &QListWidget::currentRowChanged, [&](int i){
            stackLayout->setCurrentIndex(i);
            });
    splitter->addWidget(listWidget);
    splitter->addWidget(widget);
    widget->setLayout(stackLayout);
    /* widget->setLayout(boxLayout); */
    /* widget->show(); */
    splitter->show();

    return app.exec();
}

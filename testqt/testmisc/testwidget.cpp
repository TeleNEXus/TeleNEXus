
#include <QApplication>
#include <QtWidgets>
#include <qglobal.h>
#include <qnamespace.h>
#include <QScrollArea>
#include <qscrollarea.h>
#include <QComboBox>
#include <QVBoxLayout>
#include <QPixmap>
#include <QMovie>

#include <QList>

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    QFrame* frame = new QFrame();
    QLabel* label1 = new QLabel("Text Label");
    QLabel* label2 = new QLabel();
    QLabel* label3 = new QLabel();

    QPixmap pixmap1 = QPixmap("../pict/picture_1.png").scaled(100, 100);
    label2->setPixmap(pixmap1);






    QVBoxLayout* layout = new QVBoxLayout();

    layout->addWidget(label1);
    layout->addWidget(label2);

    frame->setLayout(layout);

    frame->show();

    return app.exec();
}

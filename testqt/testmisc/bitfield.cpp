#include <QApplication>
#include <QtWidgets>
#include <qboxlayout.h>
#include <qcombobox.h>
#include <QIntValidator>
#include <qglobal.h>
#include <qregexp.h>
#include <qvalidator.h>
#include <QLabel>
#include <QPushButton>

int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    QWidget *mw = new QWidget;

    QVBoxLayout *hlayout = new QVBoxLayout();

    QLineEdit *le_in = new QLineEdit("");
    QLineEdit *le_size = new QLineEdit("-1");
    QLabel *lb_norm = new QLabel("Normalize");


    QPushButton *buttonNorm= new QPushButton("Normalize");

    hlayout->addWidget(le_in);
    hlayout->addWidget(lb_norm);
    hlayout->addWidget(le_size);
    hlayout->addWidget(buttonNorm);

    QObject::connect(buttonNorm, &QPushButton::pressed, 
            [&]()
            {
//                int size = 0;
//                size = le_size->text().toInt();
//                if( size <=0 )
//                {
//                    le_in->text().     
//                }
//                else
//                {
//                }
                quint16 data = 5;
                lb_norm->setText( 
                        QString("%1").arg(data, 16, 2, QChar('0'))
//                        QString("%1").arg(data)
                        );
            });
    mw->setLayout( hlayout );        


    mw->show();
    return app.exec();
}

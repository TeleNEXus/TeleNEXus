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
    QLineEdit *le_in = new QLineEdit("0001af 151 dfffe ed 123");
    QLineEdit *le_expr_rmove = new QLineEdit("^[0]{1,}|[ ]{1,}");
    QLineEdit *le_expr_find = new QLineEdit("[^0-9,a-f,A-F]{1,}");
    QLabel *lb_res_remove = new QLabel("remove result");
    QLabel *lb_res_find = new QLabel("find result");

    QLabel *lb_case = new QLabel("case");

    QPushButton *buttonRemove = new QPushButton("Remove");
    QPushButton *buttonFind= new QPushButton("Find");
    QPushButton *buttonCase= new QPushButton("LowCase");

    hlayout->addWidget(le_in);
    hlayout->addWidget(le_expr_rmove);
    hlayout->addWidget(lb_res_remove);
    hlayout->addWidget(buttonRemove);
    hlayout->addWidget(le_expr_find);
    hlayout->addWidget(lb_res_find);
    hlayout->addWidget(buttonFind);
    hlayout->addWidget(lb_case);
    hlayout->addWidget(buttonCase);
    
    QObject::connect(buttonRemove, &QPushButton::pressed, 
            [&]()
            {
                lb_res_remove->setText(
                        le_in->text().
                            remove(QRegExp(le_expr_rmove->text()))
                        ); 
            });


    QObject::connect(buttonFind, &QPushButton::pressed, 
            [&]()
            {
                if( lb_res_remove->text().contains(QRegExp(le_expr_find->text())) )
                {
                    lb_res_find->setText("True");
                }             
                else
                {
                    lb_res_find->setText("False");
                }
            });

    QObject::connect(buttonCase, &QPushButton::pressed, 
            [&]()
            {
                lb_case->setText(lb_res_remove->text().toLower());
                if ( (lb_case->text().size() % 2) != 0)
                {
                    lb_case->setText(lb_case->text().insert(0,QString(5, '0')));
                }
            });
//^[0]{1,}|[ ]{1,}
    mw->setLayout( hlayout );        


    mw->show();
    return app.exec();
}

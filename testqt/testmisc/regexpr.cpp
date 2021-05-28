#include <QApplication>
#include <QtWidgets>
#include <cmath>
#include <ostream>
#include <qboxlayout.h>
#include <qcombobox.h>
#include <QIntValidator>
#include <qglobal.h>
#include <qregexp.h>
#include <qvalidator.h>
#include <QLabel>
#include <QPushButton>
#include <QDebug>
#include <QTextStream>
#include <iostream>

void foo(QString& str)
{
  auto ret = [&str]()
  {
    str = "Hello!!!";
  };
  return ret();
}
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
    hlayout->addWidget(new QLineEdit(QString("Test \x04 wer")));
    
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


//==============================================================================

    auto reg_debug = []()
    {
      QTextStream out(stdout);

      /* /1* QString test_string = "Aasdf \\02 dddf \\0 safsdf \n"; *1/ */
      /* /1* QString test_string = "\\x65 \\x041f Привет \\x433 \\xf4\\xA5\\x46 \\ Av Aasdf \\v dddf \\\\v \\b aaa \\ \\\\ eee"; *1/ */
      /* /1* QString test_string = "\\x65 \\x66\\x67 Привет \\x433 \\xf4\\xA5\\x46 \\ Av Aasdf \\v dddf \\\\v \\b aaa \\ \\\\ eee"; *1/ */
      /* /1* QString test_string = "Test: \\x041f \\0 \\x65\\v   \\\\v \\\\ \\"; *1/ */
      /* QString test_string = "Test:\\x65\\v  \\0 \\\\v \\\\ \\"; */
      /*   test_string = QString::fromUtf8(test_string.toUtf8()); */
      /* out << "Test string         = " << test_string << endl; */

      /* auto list = test_string.split(QRegularExpression("(?<!\\\\)(\\\\0)")); */
      /* if(list.length() > 1) */
      /* { */
      /*   test_string = list.first(); */
      /*   test_string.append(QStringLiteral("\0")); */
      /* } */

      /* /1* if(test_string.contains(QRegularExpression("(?<!\\\\)(\\\\0)"))) *1/ */
      /* /1* { *1/ */
      /* /1*   test_string = test_string.split(QRegularExpression("(?<!\\\\)(\\\\0)")).first(); *1/ */
      /* /1*   test_string.append(QStringLiteral("\0")); *1/ */
      /* /1* } *1/ */


      /* /1* test_string.replace(QRegularExpression("(?<!\\\\)(\\\\0)"), QStringLiteral("\0")); *1/ */
      /* test_string.replace(QRegularExpression("(?<!\\\\)(\\\\v)"), QStringLiteral("[v]")); */
      /* test_string.replace(QRegularExpression("(?<!\\\\)(\\\\)(?!x)"), QStringLiteral("")); */
      /* test_string.replace(QRegularExpression("(\\\\)(?!x)"), QStringLiteral("[\\]")); */

      /* QRegularExpression re("\\\\x[0-9a-fA-F]{1,4}"); */

      /* auto matchit = re.globalMatch(test_string); */
      /* out << "//====================================Matches" << endl; */
      /* while(matchit.hasNext()) */
      /* { */
      /*   auto m = matchit.next(); */
      /*   QString ms = m.captured(); */
      /*   ms.replace(QRegularExpression("\\\\x"), QString("")); */
      /*   bool flag = false; */
      /*   quint16 b = ms.toUInt(&flag, 16); */

      /*   if(flag) */
      /*   { */
      /*     test_string.replace(m.captured(), QChar(b)); */
      /*   } */

      /* } */

      /* /1* test_string.replace(re, QString("[char code]")); *1/ */
      /* /1* test_string.replace(QRegExp("\\\\[0]{1,1}"),QChar(0)); *1/ */
      /* out << "Test string \0 replace = " << test_string << endl; */

      /* std::cout << "Test \0 string replace std = " << test_string.toStdString() << std::endl; */

      /* /1* QByteArray arrutf8 = test_string.toUtf8(); *1/ */
      /* QByteArray arr = test_string.toUtf8(); */
      /* out << "Byte array = " << arr.toHex(' ') << endl; */
      /* QString from_utf = QString::fromUtf8(arr); */
      /* out << "String from byte array = " << from_utf << endl; */

      /* for(int i = 0; i < test_string.length(); i++) */
      /* { */
      /*   out << QString("Before Char[%1] : ").arg(i) << test_string.at(i) << */ 
      /*     "\t code " << test_string.at(i).unicode() << */
      /*     QString("\t After Char[%1] : ").arg(i) << from_utf.at(i) << */ 
      /*     "\t code " << from_utf.at(i).unicode() << */
      /*     endl; */
      /* } */

      struct asdf
      {
        QString fname;
        QStringList params;
      }action_struct;

      /* QString action_string = "  function  (p1() , p2)"; */
      QString action_string = "  function  (p1() , p2)()";
      /* QString action_string = "  function  "; */
      action_string.remove(" ");
      action_struct.fname = action_string;
      action_struct.fname.remove(QRegularExpression("\\(.*"));
      action_string.remove(QRegularExpression("^[^\\(\\)]+"));
      action_string.remove(QRegularExpression("(^\\()|(\\)$)"));
      action_struct.params = action_string.split(",");
      qDebug() << "action function = " << action_struct.fname;
      qDebug() << "action params   = " << action_struct.params;



      /* split = split.split(","); */
      /* qDebug() << "split param = " << split; */

    };

    /* printf("asdfsdf \\sdf \? \n"); */
    /* std::cout<< "std::cout :\t \\slash " << std::endl; */


    reg_debug();


//==============================================================================





    /* return app.exec(); */
}

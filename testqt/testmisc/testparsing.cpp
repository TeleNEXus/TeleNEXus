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

int main(int, char**)
{

  QTextStream os(stdout);


  auto clean_space = [](const QString& _instr, const QChar& _ignor_border, bool* _ef = nullptr)
  {
    enum
    {
      ST_OUT_STRING,
      ST_IN_STRING
    } state = ST_OUT_STRING;

    const QChar space(' ');
    const QChar slash('\\');

    QChar prch;

    QString out;

    auto ret = [&_ef](const QString& _out, bool _flag)
    {
      if(_ef) *_ef = _flag;
      return _out;
    };

    for(int i = 0; i < _instr.size(); i++)
    {
      QChar ch = _instr[i];
      switch(state)
      {
      case ST_OUT_STRING:
        if(ch == _ignor_border) 
        {
          state = ST_IN_STRING;
          out.append(ch);
        }
        else
        {
          if(ch != space) out.append(ch);
        }
        break;

      case ST_IN_STRING:
        if((ch == _ignor_border)&&(prch != slash))
        {
          state = ST_OUT_STRING;
        }
        out.append(ch);
        break;
      }
      prch = ch;
    };

    if(state == ST_IN_STRING) return ret(QStringLiteral(), false);
    return ret(out.replace(QString("\\%1").arg(_ignor_border), _ignor_border), true);
  };



  const QString ss1 = "'asdf asdf    '";
  const QString ss2 = "asdf       '   asdf   '";
  const QString ss3 = "asdf 'asdf' asdf";
  const QString ss4 = QStringLiteral("asdf 'asdf   \\'  asdf   ' asdf asdf");


  /* qDebug() << "   Clean string = " << clean_space(ss1, '\''); */
  /* qDebug() << "   Clean string = " << clean_space(ss2, '\''); */
  /* qDebug() << "   Clean string = " << clean_space(ss3, '\''); */
  /* qDebug() << "   Clean string = " << clean_space(ss4, '\''); */

  os << "   Clean string = " << clean_space(ss1, '\'') << endl;
  os << "   Clean string = " << clean_space(ss2, '\'') << endl;
  os << "   Clean string = " << clean_space(ss3, '\'') << endl;
  os << "   Clean string = " << clean_space(ss4, '\'') << endl;

  const QString ss11 = "\"asdf asdf    \"";
  const QString ss12 = "asdf       \"   asdf   \"";
  const QString ss13 = "asdf 'asdf' asdf";
  const QString ss14 = QStringLiteral("asdf \"asdf   \\\"  asdf   \" asdf \"asdf");

  /* qDebug() << "   Clean string = " << clean_space(ss11, '"'); */
  /* qDebug() << "   Clean string = " << clean_space(ss12, '"'); */
  /* qDebug() << "   Clean string = " << clean_space(ss13, '"'); */
  /* qDebug() << "   Clean string = " << clean_space(ss14, '"'); */

  os << "   Clean string = " << clean_space(ss11, '"') << endl;
  os << "   Clean string = " << clean_space(ss12, '"') << endl;
  os << "   Clean string = " << clean_space(ss13, '"') << endl;
  os << "   Clean string = " << clean_space(ss14, '"') << endl;



  auto pars = 
    [&clean_space](
        const QString& _input, 
        std::function<void(const QString& _error)> _fr = [](const QString&){})
  {
    auto ret_ok = [](const QString& _func, const QStringList& _params)
    {
      QStringList ret;
      if(!_func.isNull())
      {
        ret << _func << _params;
      }
      return ret;
    };

    auto ret_wrong = [&_fr](const QString& _errStr)
    {
      _fr(_errStr);
      return QStringList();
    };

    auto get_func = [](const QString& _in)
    {
      QString out;
      for(int i = 0; i < _in.size(); i++)
      {
        if(_in.at(i) == QStringLiteral("(")) break;
        out.append(_in.at(i));
      }
      return out;
    };

    auto get_params = [](const QString& _in, bool* _flag)
    {
      int bc = 0;
      QString param;
      QStringList params;

      for(int i=0; i < _in.size(); i++)
      {
        QChar ch = _in.at(i);
        if(ch == QStringLiteral("("))
        {
          bc++;
          if(bc > 1)
          {
            param.append(ch);
          }
          continue;
        }

        if(ch == QStringLiteral(")"))
        {
          bc--;
          if(bc == 0)
          {
            if(!param.isNull())
            {
              params << param.remove(
                  QRegularExpression("^'|'$"));
            }
            break;
          }
          else
          {
            param.append(ch);
            continue;
          }
        }

        if(ch == QStringLiteral(","))
        {
          if(bc > 1)
          {
            param.append(ch);
          }
          else
          {
            if(!param.isNull()) 
              params << param.remove(
                  QRegularExpression("^'|'$"));
            param.clear();
          }
        }
        else
        {
          param.append(ch);
        }
      }

      if((bc != 0)) 
      {
        *_flag = false;
        return QStringList();
      }

      *_flag = true;
      return params;
    };
    QString func = get_func(_input);


    if(func.isNull())
    {
      return ret_wrong("No function name");
    }

    QString params_string = _input.right(_input.size() - func.size());

    bool flag = false;

    params_string = clean_space(params_string, QChar('\''), &flag);

    if(!flag) 
    {
      return ret_wrong(QString("String border error"));
    }


    auto params = get_params(params_string, &flag);

    if(!flag) return ret_wrong(QString("Close bracket error"));
    return ret_ok(func, params);
  };


  auto parse_req = [&pars](const QString& _str)
  {
    qDebug() << "--------------------------------------";
    QString parse_error;
    auto ef = [&parse_error](const QString& _err)
    {
      parse_error  = QString("%1").arg(_err);
      /* qDebug() << "     parse error : " << _err; */
    };
    qDebug() << "parse string : " << _str;
    auto res = pars(_str, ef);
    qDebug() << "parse error  : " << parse_error;
    qDebug() << "parse result : " << res;
  };


  const QString ps1 = QStringLiteral("func (p1('Param 'test string') ,p2, p3, 'Param test string')");
  const QString ps2 = QStringLiteral("func (p1(' test' string') ,p2, p3, 'test string')");
  const QString ps3 = QStringLiteral("f unc (p1(p11(p111, p112()), p12), p2, p3(p31,    p32) )()");
  const QString ps4 = QStringLiteral("func (p1(,p2(, p3");
  const QString ps5 = QStringLiteral("()");

  qDebug() << "---------------------------------pars";
  /* qDebug() << pars(ps1); */
  /* qDebug() << pars(ps2); */
  /* qDebug() << pars(ps3); */
  /* parse_req(ps1); */
  /* parse_req(ps2); */
  /* parse_req(ps3); */
  /* parse_req(ps4); */
  qDebug() << "------------------------ parse right";
  parse_req("func");
  parse_req("func()");
  parse_req("func(p1)");
  parse_req("func(p1, p2)");
  parse_req("func(p1(p11, p12), p2)");
  parse_req("func('string param 1', 'string param 2')");
  parse_req("func(p1('string param 1', 'string param 2'), p2)");
  parse_req("func(p1('string \\'param 1\\'', 'string param 2'), p2)");

  qDebug() << "------------------------ parse wrong";
  parse_req("func(");
  parse_req("func())");
  parse_req("func(p1,)");
  parse_req("func(p1,,,,)");
  parse_req("func(p1(, p2)");
  parse_req("func(p1(p11), p12), p2)");
  parse_req("func(p1('string 'param 1   ', 'string param 2'), p2)");
  return 0;
}


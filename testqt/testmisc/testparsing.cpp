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

  const QString ps1 = QStringLiteral("func (p1,p2, p3)");
  const QString ps2 = QStringLiteral("f unc (p1(p11(p111, p112()), p12), p2, p3(p31,    p32) )()");
  const QString ps3 = QStringLiteral("func ");


  auto pars = [](const QString& _input)
  {
    QString localin = _input;

    auto ret = [](const QString& _func, const QStringList& _params)
    {
      QStringList ret;
      if(!_func.isNull())
      {
        ret << _func << _params;
      }
      return ret;
    };

    qDebug() << "---------------------------Parse start";
    qDebug() << "pars 0 : " << localin;

    localin.remove(' ');

    qDebug() << "pars 1 : " << localin;

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

    QString func = get_func(localin);
    qDebug() << "pars 2 : " << func;

    localin.remove(0, func.size());
    qDebug() << "pars 3 : " << localin;


    auto get_params = [](const QString& _in)
    {
      int bc = 0;
      QString param;
      QStringList params;

      for(int i=0; i < _in.size(); i++)
      {
        if(_in.at(i) == QStringLiteral("("))
        {
          bc++;
          if(bc > 1)
          {
            param.append(_in.at(i));
          }
          continue;
        }


        if(_in.at(i) == QStringLiteral(")"))
        {
          bc--;
          if(bc == 0)
          {
            if(!param.isNull())
            {
              params << param;
            }
            break;
          }
          else
          {
            param.append(_in.at(i));
            continue;
          }
        }

        if(_in.at(i) == QStringLiteral(","))
        {
          if(bc > 1)
          {
            param.append(_in.at(i));
          }
          else
          {
            params << param;
            param.clear();
          }
        }
        else
        {
          param.append(_in.at(i));
        }
      }

      return params;
    };

    auto retlist = ret(func, get_params(localin));
    /* qDebug() << "pars 4 : "<< get_params(localin); */
    qDebug() << retlist;

    qDebug() << "---------------------------Parse stop";
    return retlist;
  };

  pars(ps1);
  pars(ps2);
  pars(ps3);
  return 0;
}


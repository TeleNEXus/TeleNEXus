/* 
 * TeleNEXus is a simple SCADA programm
 *
 * Copyright (C) 2020 Sergey S. Kuzmenko
 *
 * This file is part of TeleNEXus.
 *
 * TeleNEXus is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * TeleNEXus is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with TeleNEXus.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <QDebug>
#include <QWidget>
#include <QApplication>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QCommandLineParser>



#include <csignal>


#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QListWidget>
#include <QFileDialog>
#include <QDir>
#include <QProcess>


//==============================================================================main
QWidget* buildWindow();
int main(int argc, char *argv[])
{
  QApplication app(argc, argv);
  auto widget = buildWindow();
  widget->setWindowTitle("TeleNEXus launcher");


  QObject::connect(&app, &QApplication::aboutToQuit,
      []()
      {
        qDebug() << "Quit from TeleNEXus launcher.";
      });


  widget->show();
  return app.exec();
}

QWidget* buildWindow()
{
  QWidget* widget = new QWidget();

  QPushButton* buttonAdd = new QPushButton("Add");
  QPushButton* buttonRemove = new QPushButton("Remove");
  QPushButton* buttonLaunch = new QPushButton("Launch");


  QListWidget* projectList = new QListWidget();


  QHBoxLayout* mainLayout = new QHBoxLayout();
  QVBoxLayout* buttonsLayout = new QVBoxLayout();

  buttonsLayout->addWidget(buttonAdd);
  buttonsLayout->addWidget(buttonRemove);
  buttonsLayout->addStretch();
  buttonsLayout->addWidget(buttonLaunch);




  mainLayout->addWidget(projectList);
  mainLayout->addLayout(buttonsLayout);

  widget->setLayout(mainLayout);


  QObject::connect(buttonAdd, &QPushButton::pressed, 
      [widget]()
      {
        qDebug() << "Button 'Add' pressed";
        auto file = 
          QFileDialog::getOpenFileName(
              widget, 
              QStringLiteral("Open main xml file"), 
              QString("/home/serg/pprj/tnex/xmltestprj/test1/main.xml"), 
              QStringLiteral("Main file (main.xml)"));
        if(file.isNull())
        {
          qDebug() << "File is not selected.";
        }
        else
        {
          qDebug() << QString("File '%1' is selected").arg(file);
        }

        QFileInfo fileinfo(file);
        qDebug() << QString("Project path '%1'").arg(fileinfo.absolutePath());
        if(
            QProcess::startDetached(QString(
            "../../tnexcore/__builds/tnex --xmlpath %1").arg(fileinfo.absolutePath()))
            /* QProcess::startDetached( */
            /* "../../tnexcore/__builds/tnex", */ 
            /* QStringList() << QString("--xmlpath %1").arg(fileinfo.absoluteFilePath())) */
          )
        {
          qDebug() << "Is started...";
        }
        else
        {
          qDebug() << "Start error";
        }
      });




  return widget;
}



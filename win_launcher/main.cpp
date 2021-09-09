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
#include <QTextStream>
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

        /* QProcess::startDetached( */
        /*     "cmd /k start \"tnex\" .\\tnex.exe --xmlpath ..\\xmltestprj\\test1\\"); */

#define __smHistoryFile QStringLiteral("./projectlist.ini")

//==============================================================================main
QWidget* buildWindow(const QApplication& _app);
int main(int argc, char *argv[])
{

#ifdef Q_OS_WIN
            qDebug() << "TeleNEXus launcher started in Windows";
#endif
#ifdef Q_OS_LINUX
            qDebug() << "TeleNEXus launcher started in Linux";
#endif

  QApplication app(argc, argv);

  /* QProcess prc; */

  auto widget = buildWindow(app);

  widget->setWindowTitle("TeleNEXus launcher");




  widget->show();
  return app.exec();
}

QWidget* buildWindow(const QApplication& _app)
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


  auto load_project_list = 
    [projectList](QFile& _file)
    {
      QTextStream text_stream(&_file);
      QString str;
      while(text_stream.readLineInto(&str))
      {
        if(QFileInfo::exists(str))
        {
          projectList->addItem(str);
        }
      }
    };

  if(QFileInfo::exists(__smHistoryFile))
  {
    QFile file(__smHistoryFile);
    if(file.open(QFile::OpenModeFlag::ReadOnly))
    {
      load_project_list(file);
    }
  }

  QObject::connect(buttonAdd, &QPushButton::pressed, 
      [widget, projectList]()
      {

        qDebug() << "Button 'Add' pressed";


        auto file = 
          QFileDialog::getOpenFileName(
              widget, 
              QStringLiteral("Open main xml file"), 
              QString(""), 
              QStringLiteral("Main file (main.xml);;xml files (*.xml)"));

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
        qDebug() << QString("Project main file '%1'").arg(fileinfo.fileName());
        if(!file.isNull()) projectList->addItem(file);
      });


  QObject::connect(buttonRemove, &QPushButton::pressed,
      [projectList]()
      {
        qDebug() << "Button 'Remove' was pressed.";
        qDebug() << QString("Current project row is %1")
          .arg(projectList->currentRow());
        auto item = projectList->takeItem(projectList->currentRow());
        if(item != nullptr) delete item;
      });

  QObject::connect(buttonLaunch, &QPushButton::pressed,
      [projectList]()
      {
        qDebug() << "Button 'Launch' was pressed.";

        auto list_item = projectList->currentItem();
        if(list_item == nullptr) return;

        QString project_string = projectList->currentItem()->text();
        qDebug() << "Project string = " << project_string;

        QFileInfo fileinfo(project_string);


        qDebug() << "Project path = " << fileinfo.absolutePath();
        qDebug() << "Profect main file name = " << fileinfo.fileName();

#ifdef Q_OS_WIN
        //windows
        /* QProcess::startDetached( */
        /*     "cmd /k start \"tnex\" .\\tnex.exe --xmlpath ..\\xmltestprj\\test1\\"); */

        QProcess::startDetached(
            QString("cmd /k start \"tnex\" .\\tnex.exe --xmlpath %1 --xmlfile %2")
            .arg(fileinfo.absolutePath())
            .arg(fileinfo.fileName()));
#else

        QProcess::startDetached(
            QString("../tnex --xmlpath %1 --xmlfile %2")
            .arg(fileinfo.absolutePath())
            .arg(fileinfo.fileName()));
#endif
      });

  QObject::connect(&_app, &QApplication::aboutToQuit,
      [projectList]()
      {

        auto write_list_data = 
          [projectList](QFile& _file)
          {
            QTextStream text_stream(&_file);
            for(int i = 0; i < projectList->count(); i++)
            {
              text_stream << projectList->item(i)->text() << "\n";
            }
          };

        if(projectList->count() != 0)
        {
          QFile::remove(__smHistoryFile);
          QFile file(__smHistoryFile);
          if(file.open(QFile::OpenModeFlag::Append))
          {
            write_list_data(file);
          }
        }
        qDebug() << "Quit from TeleNEXus launcher.";
      });

  return widget;
}



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

#define __smHistoryFileName QStringLiteral("projectlist.ini")

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

  QString history_file = QString("%1/%2")
    .arg(QApplication::applicationDirPath()).arg(__smHistoryFileName);
  qDebug() << "History file = " << history_file;

  QPushButton* buttonAddDir = new QPushButton("Add project path");

  QPushButton* buttonAddFile = new QPushButton("Add pack file");

  QPushButton* buttonRemove = new QPushButton("Remove");
  QPushButton* buttonLaunch = new QPushButton("Launch");

  QListWidget* projectList = new QListWidget();

  QHBoxLayout* mainLayout = new QHBoxLayout();
  QVBoxLayout* buttonsLayout = new QVBoxLayout();

  buttonsLayout->addWidget(buttonAddDir);
  buttonsLayout->addWidget(buttonAddFile);
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

  if(QFileInfo::exists(history_file))
  {
    QFile file(history_file);
    if(file.open(QFile::OpenModeFlag::ReadOnly))
    {
      load_project_list(file);
    }
  }

  QObject::connect(buttonAddDir, &QPushButton::pressed, 
      [widget, projectList]()
      {
        qDebug() << "Button 'Add path' pressed";

        auto dir = QFileDialog::getExistingDirectory(widget, QString("Open project directory"));

        /* auto file = */ 
        /*   QFileDialog::getOpenFileName( */
        /*       widget, */ 
        /*       QStringLiteral("Open main xml file"), */ 
        /*       QString(""), */ 
        /*       QStringLiteral("Main file (main.xml);;xml files (*.xml)")); */

        if(dir.isNull())
        {
          qDebug() << "Directory is not selected.";
        }
        else
        {
          qDebug() << QString("Directory '%1' is selected").arg(dir);
        }

        QFileInfo fileinfo(dir);

        if(!dir.isNull()) projectList->addItem(dir);
      });

  QObject::connect(buttonAddFile, &QPushButton::pressed, 
      [widget, projectList]()
      {

        qDebug() << "Button 'Add pack file' pressed";

        auto file = 
          QFileDialog::getOpenFileName(
              widget, 
              QStringLiteral("Open main xml file"), 
              QString(""), 
              QStringLiteral("Pack files (*.pack);;All files (*.*)"));

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

        QString project_string = list_item->text();

        qDebug() << "Launch: " << project_string;

        QFileInfo fi(project_string);
        if(fi.isDir()) project_string += "/";


#ifdef Q_OS_WIN
        //windows
        /* QProcess::startDetached( */
        /*     "cmd /k start \"tnex\" .\\tnex.exe --xmlpath ..\\xmltestprj\\test1\\"); */

        qDebug() << "Start from string : " << project_string;
        QProcess::startDetached(
            /* QString("cmd /C start /Wait \"tnex\" .\\tnex.exe %1") */
            /* QString("cmd /C start tnex.exe %1") */
            QString("cmd /C start tnex.exe %1")
            .arg(project_string));

        /* QProcess::startDetached( */
        /*     QString("tnex.exe %1") */
        /*     .arg(project_string)); */
#else

        QProcess::startDetached(
            QString("tnex  %1")
            .arg(project_string));
#endif
      });

  QObject::connect(&_app, &QApplication::aboutToQuit,
      [projectList, history_file]()
      {

        /* auto write_list_data = */ 
        /*   [projectList](QFile& _file) */
        /*   { */
        /*     QTextStream text_stream(&_file); */
        /*     for(int i = 0; i < projectList->count(); i++) */
        /*     { */
        /*       text_stream << projectList->item(i)->text() << "\n"; */
        /*     } */
        /*   }; */


        /* if(projectList->count() != 0) */
        /* { */
        /*   QFile::remove(history_file); */
        /*   QFile file(history_file); */
        /*   if(file.open(QFile::OpenModeFlag::Append)) */
        /*   { */
        /*     write_list_data(file); */
        /*   } */
        /* } */
        qDebug() << "Quit from TeleNEXus launcher++++.";
      });

  QObject::connect(&_app, &QApplication::lastWindowClosed, 
      [projectList, history_file]()
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


        QFile::remove(history_file);
        QFile file(history_file);
        if(file.open(QFile::OpenModeFlag::Append))
        {
          qDebug() << QString( "Can't open ini file '%1' for write." ).arg(history_file);
        }
        if(projectList->count() != 0)
        {
          if(file.isOpen())
          {
            write_list_data(file);
          }
          else
          {
            qDebug() << QString("Can't aopen ini file '%1'").arg(history_file);
          }
        }
        else
        {
          qDebug() << "Project list count = 0";
        }
        qDebug() << "Last widndow closed";
      });

  return widget;
}



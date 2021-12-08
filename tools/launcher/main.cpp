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
#include <QLocale>
#include <QTranslator>
#include <QCheckBox>

#define __smHistoryFileName QStringLiteral("projectlist.ini")
#define __smLinuxTnexHomeDir ".tnex"

//==============================================================================main
QWidget* buildWindow(const QApplication& _app, const QString& _historyFile);
int main(int argc, char *argv[])
{

#ifdef Q_OS_WIN
  /* qDebug().noquote() << "TeleNEXus launcher started in Windows"; */
  /* QString history_file = QString("%1/%2") */
  /*   .arg(QApplication::applicationDirPath()).arg(__smHistoryFileName); */

  qDebug() << "Window home path: " << QDir::homePath();

#endif
#ifdef Q_OS_LINUX
  qDebug().noquote() << "TeleNEXus launcher started in Linux";

#endif

  QString history_file;

  {
    QDir home_dir(QString("%1/%2")
        .arg(QDir::homePath()).arg(__smLinuxTnexHomeDir));

    qDebug().noquote() << 
      QString("TeleNEXus home dir '%1'").arg(home_dir.absolutePath());

    if(!home_dir.exists())
    {
      qDebug() << "Create home dir";
      home_dir.mkdir(home_dir.absolutePath());
    }

    history_file = QString("%1/%2")
      .arg(home_dir.absolutePath()).arg(__smHistoryFileName);
  }


  qDebug().noquote() << "History file = " << history_file;

  QTranslator translator;

  QApplication app(argc, argv);

  QLocale locale = QLocale::system();

  QString translate_file;

  if(locale.country() == QLocale::Country::Russia)
  {
    qDebug().noquote() << "Set language 'Russian'";
    translate_file = "main_ru.qm";
  }
  else
  {
    qDebug().noquote() << "Set lanuage 'English'";
    translate_file = "main_en.qm";
  }


  qDebug() <<"Application path : " << QApplication::applicationDirPath();

  translator.load(translate_file, QApplication::applicationDirPath());

  app.installTranslator(&translator);

  auto widget = buildWindow(app, history_file);

  widget->setWindowTitle("TeleNEXus launcher");
   
  widget->show();
  widget->resize(widget->width() + 200, widget->height());
  return app.exec();
}

QWidget* buildWindow(const QApplication& _app, const QString& _history_file)
{
  QWidget* widget = new QWidget();

  QPushButton* buttonAddDir = new QPushButton(QObject::tr("Add project path"));
  QPushButton* buttonAddFile = new QPushButton(QObject::tr("Add pack file"));
  QPushButton* buttonRemove = new QPushButton(QObject::tr("Remove"));
  QPushButton* buttonLaunch = new QPushButton(QObject::tr("Launch"));
  QCheckBox* terminalCheckBox = new QCheckBox(QObject::tr("Launch in terminal"));

  QListWidget* projectList = new QListWidget();

  QHBoxLayout* mainLayout = new QHBoxLayout();
  QVBoxLayout* buttonsLayout = new QVBoxLayout();

  buttonsLayout->addWidget(buttonAddDir);
  buttonsLayout->addWidget(buttonAddFile);
  buttonsLayout->addWidget(buttonRemove);
  buttonsLayout->addWidget(terminalCheckBox);
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

  if(QFileInfo::exists(_history_file))
  {
    QFile file(_history_file);
    if(file.open(QFile::OpenModeFlag::ReadOnly))
    {
      load_project_list(file);
    }
  }

  QObject::connect(buttonAddDir, &QPushButton::pressed, 
      [widget, projectList]()
      {
        qDebug().noquote() << "Button 'Add path' pressed";

        auto dir = 
          QFileDialog::getExistingDirectory(
              widget, QString("Open project directory"));

        if(dir.isNull())
        {
          qDebug().noquote() << "Directory is not selected.";
        }
        else
        {
          qDebug().noquote() << QString("Directory '%1' is selected").arg(dir);
        }

        QFileInfo fileinfo(dir);

        if(!dir.isNull()) projectList->addItem(dir);
      });

  QObject::connect(buttonAddFile, &QPushButton::pressed, 
      [widget, projectList]()
      {

        auto file = 
          QFileDialog::getOpenFileName(
              widget, 
              QStringLiteral("Open main xml file"), 
              QString(""), 
              QStringLiteral("Pack files (*.pack);;All files (*.*)"));

        QFileInfo fileinfo(file);

        if(!file.isNull()) projectList->addItem(file);
      });

  QObject::connect(buttonRemove, &QPushButton::pressed,
      [projectList]()
      {
        auto item = projectList->takeItem(projectList->currentRow());
        if(item != nullptr) delete item;
      });

  QObject::connect(buttonLaunch, &QPushButton::pressed,
      [projectList, terminalCheckBox]()
      {

        auto list_item = projectList->currentItem();
        if(list_item == nullptr) return;

        QString project_string = list_item->text();

        qDebug().noquote() << "Launch: " << project_string;

        QFileInfo fi(project_string);
        if(fi.isDir()) project_string += "/";


#ifdef Q_OS_WIN

        qDebug().noquote() << "Start from string : " << project_string;

        if(terminalCheckBox->isChecked())
        {
          QProcess::startDetached(
              QString("cmd /C start tnex.exe %1")
              .arg(project_string));
        }
        else
        {
          QProcess::startDetached(
              QString("tnex.exe %1")
              .arg(project_string));
        }
#else
        /* Q_UNUSED(terminalCheckBox); */
        if(terminalCheckBox->isChecked())
        {
          QProcess::startDetached(
              /* QString("xterm -hold -e tnex  %1 &") */
              /* QString("xterm -sl 3000 -e tnex  %1 &") */
              QString("xterm -e tnex  %1 &")
              .arg(project_string));
        }
        else
        {
          QProcess::startDetached(
              QString("tnex  %1")
              .arg(project_string));
        }
#endif
      });

  QObject::connect(&_app, &QApplication::aboutToQuit,
      []()
      {
        qDebug().noquote() << "Quit from TeleNEXus launcher.";
      });

  QObject::connect(&_app, &QApplication::lastWindowClosed, 
      [projectList, _history_file]()
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

        QFile::remove(_history_file);
        QFile file(_history_file);

        if(!file.open(QFile::OpenModeFlag::Append))
        {
          qDebug().noquote() << 
            QString( "Can't open ini file '%1' for write." ).arg(_history_file);
          return;
        }

        if(projectList->count() != 0)
        {
          if(file.isOpen())
          {
            write_list_data(file);
          }
          else
          {
            qDebug().noquote() << 
              QString("Can't open ini file '%1'").arg(_history_file);
          }
        }
      });

  return widget;
}



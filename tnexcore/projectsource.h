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
#ifndef PROJECTSOURCE_H_
#define PROJECTSOURCE_H_

#include <QIODevice>
#include <QDir>
#include <QSharedPointer>

//==============================================================================
class LIProjectSource
{
public:
  LIProjectSource(){}
  virtual ~LIProjectSource(){}
  virtual QSharedPointer<QIODevice> getFileDevice(const QString& _fileName) const = 0;
};

//==============================================================================
/* class LCPacketProjectSource : public LIProjectSource */
/* { */
/* private: */
/*   LCPacketProjectSource(){} */
/* public: */
/*   virtual ~LCPacketProjectSource(){} */
/*   static QSharedPointer<LCPacketProjectSource> */ 
/*     create(QSharedPointer<QMap<QString,QByteArray>> _filesMap); */

/*   virtual QSharedPointer<QIODevice> */ 
/*     getFileDevice(const QString& _fileName) const override; */
/* }; */

//==============================================================================
class LCDirProjectSource : public LIProjectSource
{
private:
  QDir mProjectDir;
  LCDirProjectSource() = delete;
  LCDirProjectSource(const QDir& _projectDir);
public:
  virtual ~LCDirProjectSource();

  static QSharedPointer<LCDirProjectSource> 
    create(const QString& _prjPath, QString& _msg);

  virtual QSharedPointer<QIODevice> 
    getFileDevice(const QString& _fileName) const override;
};

#endif


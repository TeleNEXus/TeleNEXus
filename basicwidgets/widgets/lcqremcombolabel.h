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
#ifndef LCQREMCOMBOLABEL_H
#define LCQREMCOMBOLABEL_H

#include "lqextendevent.h"
#include "LIDataFormatter.h"
#include "LIRemoteDataReader.h"
#include "LIRemoteDataSource.h"

#include <QStackedWidget>
#include <qnamespace.h>

class LIMovieAccess;

class LCQRemComboLabel : public QStackedWidget 
{
  Q_OBJECT
private:
    void* mpOwnData;    //Собственные данные.
    QSharedPointer<LIRemoteDataReader>  mDataReader;
    QSharedPointer<LIDataFormatter> mspFormatter;
public:

    explicit LCQRemComboLabel(   
        const QString&                      _dataName,
        QSharedPointer<LIRemoteDataSource>  _dataSource,
        QSharedPointer<LIDataFormatter>     _formatter,
        QWidget* _parent = nullptr);

    virtual ~LCQRemComboLabel();

    void addItem(QWidget* _widget, const QString&  _val);
    void addItemUndef(QWidget* _widget);
    void addItemWrong(QWidget* _widget);
private:
    bool mFlagActive = false;
    void setActive(bool _flag);
    virtual bool event(QEvent *e) override;
};

#endif // LCQREMCOMBOLABEL_H

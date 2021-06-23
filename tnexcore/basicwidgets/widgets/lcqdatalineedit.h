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
#ifndef LCQDATALINEEDIT_H_
#define LCQDATALINEEDIT_H_

#include <QLineEdit>
#include <QSharedPointer>

class LIRemoteDataReader;
class LIRemoteDataWriter;
class LIDataFormatter;
class LIKeyboard;

class LCQDataLineEdit : public QLineEdit
{
private:
  void* mpLocal;
public:
  LCQDataLineEdit() = delete;
  LCQDataLineEdit(
      QSharedPointer<LIRemoteDataReader>  _dataReader,
      QSharedPointer<LIDataFormatter>     _readFormatter,
      QSharedPointer<LIRemoteDataWriter>  _dataWriter,
      QSharedPointer<LIDataFormatter>     _writeFormatter,
      QWidget* _parent=nullptr);

  virtual ~LCQDataLineEdit();
  void installKeyboard(QSharedPointer<LIKeyboard> _keyboard);
  virtual bool event(QEvent* _event) override;
private:
  void setActive(bool _flag);
};
#endif /* LCQDATALINEEDIT_H_ */
